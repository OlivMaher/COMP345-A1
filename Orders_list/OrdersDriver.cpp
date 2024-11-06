// OrdersDriver.cpp

#include <iostream>
#include "Orders.h"

using namespace std;

/**
 * @brief Function to test OrdersList functionality
 */
void testOrdersLists() {
    cout << "=== Testing Orders List ===" << endl;

    // Create orders of every kind
    Order* deployOrder = new Deploy();
    Order* advanceOrder = new Advance();
    Order* bombOrder = new Bomb();
    Order* blockadeOrder = new Blockade();
    Order* airliftOrder = new Airlift();
    Order* negotiateOrder = new Negotiate();

    // Create an OrdersList and add orders
    OrdersList ordersList;
    ordersList.add(deployOrder);
    ordersList.add(advanceOrder);
    ordersList.add(bombOrder);
    ordersList.add(blockadeOrder);
    ordersList.add(airliftOrder);
    ordersList.add(negotiateOrder);

    // Display the OrdersList
    cout << ordersList << endl;

    // Move an order
    cout << "Moving 'Bomb' order to the first position." << endl;
    ordersList.move(2, 0); // Move Bomb order from index 2 to index 0
    cout << ordersList << endl;

    // Remove an order
    cout << "Removing 'Blockade' order." << endl;
    ordersList.remove(3); // Remove the order at index 3
    cout << ordersList << endl;

    // Execute all orders
    cout << "Executing all orders:" << endl;
    const vector<Order*>& orders = ordersList.getOrders();
    for (Order* order : orders) {
        order->execute();
        cout << *order << endl;
    }

    // Orders are deleted in OrdersList destructor
    cout << "=== End of Orders List Test ===" << endl;
}