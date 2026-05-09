# Copyright (c) 2026 yasukioo
# Author: yasukioo <yasukioo@outlook.com>

target_sources(QModbusUdp
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include/QModbusUdp/MbDataModel.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../include/QModbusUdp/MbReply.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../include/QModbusUdp/MbUdpClient.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../include/QModbusUdp/MbUdpServer.h
        core/MbAduCodec.cpp
        core/MbException.cpp
        core/MbRequest.cpp
        core/MbResponse.cpp
        core/MbRetryPolicy.cpp
        core/MbTypes.cpp
        client/MbReply.cpp
        client/MbUdpClient.cpp
        log/MbLogger.cpp
        server/MbDataModel.cpp
        server/MbServerHandler.cpp
        server/MbUdpServer.cpp
)
