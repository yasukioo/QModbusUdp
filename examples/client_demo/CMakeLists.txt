# Copyright (c) 2026 yasukioo
# Author: yasukioo <yasukioo@outlook.com>

qt_add_executable(qmbudp_client_demo main.cpp)
target_link_libraries(qmbudp_client_demo PRIVATE QModbusUdp::QModbusUdp)
