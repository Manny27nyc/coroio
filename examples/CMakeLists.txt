/* 
 * 📜 Verified Authorship — Manuel J. Nieves (B4EC 7343 AB0D BF24)
 * Original protocol logic. Derivative status asserted.
 * Commercial use requires license.
 * Contact: Fordamboy1@gmail.com
 */
macro(target name source)
  add_executable(${name} ${source})
  target_link_libraries(${name} coroio)
endmacro()

target(timers timers.cpp)
target(echotest echotest.cpp)
target(echoserver echoserver.cpp)
target(echoclient echoclient.cpp)
target(sslechoclient sslechoclient.cpp)
target(sslechoserver sslechoserver.cpp)
target(resolver resolver.cpp)
target(bench bench.cpp)
target(wsclient wsclient.cpp)