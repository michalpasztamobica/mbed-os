
####################
# UNIT TESTS
####################

set(unittest-includes ${unittest-includes}
  .
  ..
)

set(unittest-sources
  ../features/device_key/source/DeviceKey.cpp
)

set(unittest-test-sources
  features/device_key/test_DeviceKey.cpp
  ../features/nanostack/coap-service/test/coap-service/unittest/stub/mbedtls_stub.c
  stubs/cipher_stub.c
  stubs/cmac_stub.c
  stubs/KVMap_stub.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEVICEKEY_ENABLED -DDEVICE_FLASH -DCOMPONENT_FLASHIAP -DDEVICE_TRNG")