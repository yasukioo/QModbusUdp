# QModbusUdp

[中文](#中文说明) | [English](#english)

## 中文说明

QModbusUdp 是一个基于 Qt 6 的轻量级 Modbus UDP 通信库，提供客户端、服务端、数据模型、请求/响应编解码以及基础重试能力，适合在工业控制、设备联调和协议验证场景中使用。

### 功能特性

- 基于 `Qt6::Core` 与 `Qt6::Network`
- 提供 `MbUdpClient` 与 `MbUdpServer`
- 支持 Modbus 请求、响应与 ADU 编解码
- 内置 `MbDataModel`，方便维护线圈和寄存器数据
- 支持超时、重试和回复对象异步处理
- 自带示例程序与单元测试

### 目录结构

```text
include/QModbusUdp/   公共头文件
src/                  库实现
examples/             client_demo 与 server_demo
tests/                编解码、数据模型、服务端、UDP 往返测试
```

### 依赖要求

- CMake 3.21 及以上
- Qt 6.5 及以上
- C++17 编译器

### 构建方法

#### 1. 配置

```bash
cmake -S . -B build
```

如果你不需要示例或测试，可以关闭：

```bash
cmake -S . -B build -DQMODBUSUDP_BUILD_EXAMPLES=OFF -DQMODBUSUDP_BUILD_TESTS=OFF
```

#### 2. 编译

```bash
cmake --build build
```

#### 3. 运行测试

```bash
ctest --test-dir build --output-on-failure
```

### 使用方法

#### 启动服务端

服务端示例会监听 UDP `1502` 端口，为单元号 `1` 初始化 Holding Register 数据，并在收到请求或线圈变化时输出日志。

```bash
./build/examples/server_demo/server_demo
```

Windows 多配置生成器下通常是：

```bash
./build/examples/server_demo/Debug/server_demo.exe
```

#### 启动客户端

客户端示例会绑定本地 UDP 端口，向 `127.0.0.1:1502` 的单元号 `1` 发送读取 Holding Register 请求，并异步打印结果。

```bash
./build/examples/client_demo/client_demo
```

Windows 多配置生成器下通常是：

```bash
./build/examples/client_demo/Debug/client_demo.exe
```

建议先启动 `server_demo`，再运行 `client_demo`。

### 最小示例

#### 服务端

```cpp
#include <QCoreApplication>
#include <QHostAddress>
#include <QModbusUdp/MbDataModel.h>
#include <QModbusUdp/MbUdpServer.h>

using namespace QMbUdp;

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    MbUdpServer server;
    if (!server.listen(QHostAddress::AnyIPv4, 1502)) {
        return 1;
    }

    MbDataModel *model = server.dataModel(1);
    model->writeHolding(100, {0x1234, 0x5678});

    return app.exec();
}
```

#### 客户端

```cpp
#include <QCoreApplication>
#include <QHostAddress>
#include <QModbusUdp/MbReply.h>
#include <QModbusUdp/MbRequest.h>
#include <QModbusUdp/MbUdpClient.h>

using namespace QMbUdp;

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    MbUdpClient client;
    client.bind();
    client.setDefaultTimeout(800);
    client.setMaxRetries(3);

    MbReadHoldingRegRequest request(100, 2);
    MbReply *reply = client.send(request, 1, QHostAddress(QStringLiteral("127.0.0.1")), 1502);

    QObject::connect(reply, &MbReply::finished, reply, [reply] {
        reply->deleteLater();
        QCoreApplication::quit();
    });

    return app.exec();
}
```

### CMake 集成

在其他工程中链接本库时，核心目标为：

- `QModbusUdp`
- `QModbusUdp::QModbusUdp`

示例：

```cmake
target_link_libraries(your_target PRIVATE QModbusUdp::QModbusUdp)
```

### 当前测试覆盖

仓库当前包含以下测试：

- `tst_codec`
- `tst_datamodel`
- `tst_server`
- `tst_udp_roundtrip`

### 适用场景

- Qt 设备通信模块开发
- Modbus UDP 协议验证
- 本地联调与演示程序
- 需要可控数据模型的仿真服务端

## English

QModbusUdp is a lightweight Modbus UDP library built on Qt 6. It provides client and server APIs, request/response and ADU codecs, a simple data model, and retry support for industrial communication, device integration, and protocol testing workflows.

### Features

- Built on `Qt6::Core` and `Qt6::Network`
- Provides `MbUdpClient` and `MbUdpServer`
- Includes Modbus request, response, and ADU codec support
- Ships with `MbDataModel` for coils and registers
- Supports timeout, retry, and asynchronous reply handling
- Comes with examples and unit tests

### Project Layout

```text
include/QModbusUdp/   Public headers
src/                  Library implementation
examples/             client_demo and server_demo
tests/                Codec, data model, server, and UDP roundtrip tests
```

### Requirements

- CMake 3.21 or newer
- Qt 6.5 or newer
- A C++17 compiler

### Build

#### 1. Configure

```bash
cmake -S . -B build
```

To disable examples or tests:

```bash
cmake -S . -B build -DQMODBUSUDP_BUILD_EXAMPLES=OFF -DQMODBUSUDP_BUILD_TESTS=OFF
```

#### 2. Build

```bash
cmake --build build
```

#### 3. Run tests

```bash
ctest --test-dir build --output-on-failure
```

### Usage

#### Run the server example

The server example listens on UDP port `1502`, initializes holding registers for unit id `1`, and logs incoming requests and coil updates.

```bash
./build/examples/server_demo/server_demo
```

On Windows with a multi-config generator:

```bash
./build/examples/server_demo/Debug/server_demo.exe
```

#### Run the client example

The client example binds a local UDP socket, sends a read holding register request to unit id `1` at `127.0.0.1:1502`, and prints the response asynchronously.

```bash
./build/examples/client_demo/client_demo
```

On Windows with a multi-config generator:

```bash
./build/examples/client_demo/Debug/client_demo.exe
```

Start `server_demo` first, then run `client_demo`.

### Minimal Example

#### Server

```cpp
#include <QCoreApplication>
#include <QHostAddress>
#include <QModbusUdp/MbDataModel.h>
#include <QModbusUdp/MbUdpServer.h>

using namespace QMbUdp;

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    MbUdpServer server;
    if (!server.listen(QHostAddress::AnyIPv4, 1502)) {
        return 1;
    }

    MbDataModel *model = server.dataModel(1);
    model->writeHolding(100, {0x1234, 0x5678});

    return app.exec();
}
```

#### Client

```cpp
#include <QCoreApplication>
#include <QHostAddress>
#include <QModbusUdp/MbReply.h>
#include <QModbusUdp/MbRequest.h>
#include <QModbusUdp/MbUdpClient.h>

using namespace QMbUdp;

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    MbUdpClient client;
    client.bind();
    client.setDefaultTimeout(800);
    client.setMaxRetries(3);

    MbReadHoldingRegRequest request(100, 2);
    MbReply *reply = client.send(request, 1, QHostAddress(QStringLiteral("127.0.0.1")), 1502);

    QObject::connect(reply, &MbReply::finished, reply, [reply] {
        reply->deleteLater();
        QCoreApplication::quit();
    });

    return app.exec();
}
```

### CMake Integration

Link against:

- `QModbusUdp`
- `QModbusUdp::QModbusUdp`

Example:

```cmake
target_link_libraries(your_target PRIVATE QModbusUdp::QModbusUdp)
```

### Test Coverage

The repository currently includes:

- `tst_codec`
- `tst_datamodel`
- `tst_server`
- `tst_udp_roundtrip`

### Typical Use Cases

- Qt-based device communication modules
- Modbus UDP protocol verification
- Local integration and demo programs
- Simulation servers backed by a controllable data model
