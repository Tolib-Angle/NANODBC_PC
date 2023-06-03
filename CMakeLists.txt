# CMakeList.txt: проект CMake для PublishingCenter; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Добавьте источник в исполняемый файл этого проекта.
add_executable (PublishingCenter "PublishingCenter.cpp" "PublishingCenter.h" "Gateway.h" "Books/Books.cpp" "Books/Books.h" "Cash/Cash.h" "Contracts/Contracts.cpp" "Contracts/Contracts.h" "Customers/Customers.cpp" "Customers/Customers.h" "DateBaseConnect/DateBaseConnect.cpp" "DateBaseConnect/DateBaseConnect.h" "Orders/Orders.cpp" "Orders/Orders.h" "Writers/Writers.cpp" "Writers/Writers.h" "Writers/GatewayWriters.cpp" "Writers/GatewayWriters.h" "functions/functions.h" "functions/functions.cpp" "Interface/InterfaceTables.h" "Interface/InterfaceTables.cpp" "Contracts/GatewayContracts.h" "Contracts/GatewayContracts.cpp" "Customers/GatewayCustomers.h" "Customers/GatewayCustomers.cpp" "Books/GatewayBooks.h" "Books/GatewayBooks.cpp" "Orders/GatewayOrders.h" "Orders/GatewayOrders.cpp")

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
target_link_libraries(PublishingCenter nanodbc)
target_link_directories(PublishingCenter PRIVATE nanodbc)