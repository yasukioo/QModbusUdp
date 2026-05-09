# Copyright (c) 2026 yasukioo
# Author: yasukioo <yasukioo@outlook.com>

qt_add_executable(qmbudp_server_demo main.cpp)
target_link_libraries(qmbudp_server_demo PRIVATE QModbusUdp::QModbusUdp)
