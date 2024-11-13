add_executable(mytest test.c ../src/scheduling.c)

target_link_libraries(mytest PRIVATE
  pico_stdlib
  pico_multicore
  pico_async_context_freertos
  pico_time
  hardware_timer
  FreeRTOS-Kernel-Heap4
  unity
)

# Modify the below lines to enable/disable output over UART/USB.
# These are mutually exclusive, only set one.
pico_enable_stdio_uart(mytest 0)
pico_enable_stdio_usb(mytest 1)