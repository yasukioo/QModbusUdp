# Copyright (c) 2026 yasukioo
# Author: yasukioo <yasukioo@outlook.com>

qt_add_executable(tst_codec tst_codec.cpp)
target_link_libraries(tst_codec PRIVATE QModbusUdp::QModbusUdp Qt6::Test)
add_test(NAME tst_codec COMMAND tst_codec)

qt_add_executable(tst_datamodel tst_datamodel.cpp)
target_link_libraries(tst_datamodel PRIVATE QModbusUdp::QModbusUdp Qt6::Test)
add_test(NAME tst_datamodel COMMAND tst_datamodel)

qt_add_executable(tst_server tst_server.cpp)
target_link_libraries(tst_server PRIVATE QModbusUdp::QModbusUdp Qt6::Test)
add_test(NAME tst_server COMMAND tst_server)

qt_add_executable(tst_udp_roundtrip tst_udp_roundtrip.cpp)
target_link_libraries(tst_udp_roundtrip PRIVATE QModbusUdp::QModbusUdp Qt6::Test)
add_test(NAME tst_udp_roundtrip COMMAND tst_udp_roundtrip)

foreach(test_name IN ITEMS tst_codec tst_datamodel tst_server tst_udp_roundtrip)
    set(qmodbusudp_test_path_mods
        "PATH=path_list_prepend:$<TARGET_FILE_DIR:QModbusUdp>"
        "PATH=path_list_prepend:$<TARGET_FILE_DIR:Qt6::Core>"
    )
    set_tests_properties(${test_name} PROPERTIES
        ENVIRONMENT_MODIFICATION "${qmodbusudp_test_path_mods}"
    )
endforeach()
