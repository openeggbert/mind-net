add_executable(mind_net_app Main.cpp)

target_link_libraries(mind_net_app PRIVATE
        mind_net_core
        mind_net_other
)
