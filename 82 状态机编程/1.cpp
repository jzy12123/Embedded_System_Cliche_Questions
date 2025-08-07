#include <iostream>
#include <string>
using namespace std;

// 状态枚举
enum class VendingMachineState {
    IDLE,           // 空闲状态
    WAITING_FOR_COIN,   // 等待投币
    COIN_INSERTED,      // 已投币
    PRODUCT_SELECTED,    // 已选择商品
    DISPENSING,         // 出货中
    OUT_OF_STOCK        // 缺货
};

// 事件枚举
enum class Event {
    INSERT_COIN,        // 投币
    SELECT_PRODUCT,     // 选择商品
    DISPENSE_PRODUCT,   // 出货
    OUT_OF_STOCK,      // 缺货
    RESET              // 重置
};

// 自动售货机状态机类
class VendingMachine {
private:
    VendingMachineState currentState;
    int coins;
    int selectedProduct;
    bool hasStock;

public:
    VendingMachine() {
        currentState = VendingMachineState::IDLE;
        coins = 0;
        selectedProduct = -1;
        hasStock = true;
    }

    // 处理事件
    void handleEvent(Event event) {
        cout << "Current State: " << getStateString() << endl;
        cout << "Received Event: " << getEventString(event) << endl;

        switch (currentState) {
            case VendingMachineState::IDLE:
                handleIdleState(event);
                break;
            case VendingMachineState::WAITING_FOR_COIN:
                handleWaitingForCoinState(event);
                break;
            case VendingMachineState::COIN_INSERTED:
                handleCoinInsertedState(event);
                break;
            case VendingMachineState::PRODUCT_SELECTED:
                handleProductSelectedState(event);
                break;
            case VendingMachineState::DISPENSING:
                handleDispensingState(event);
                break;
            case VendingMachineState::OUT_OF_STOCK:
                handleOutOfStockState(event);
                break;
        }

        cout << "State Transition to: " << getStateString() << endl;
        cout << "------------------------" << endl;
    }

private:
    // 处理空闲状态
    void handleIdleState(Event event) {
        switch (event) {
            case Event::INSERT_COIN:
                if (hasStock) {
                    coins++;
                    currentState = VendingMachineState::COIN_INSERTED;
                    cout << "Coin inserted successfully, current coins: " << coins << endl;
                } else {
                    currentState = VendingMachineState::OUT_OF_STOCK;
                    cout << "Product out of stock!" << endl;
                }
                break;
            case Event::OUT_OF_STOCK:
                hasStock = false;
                currentState = VendingMachineState::OUT_OF_STOCK;
                cout << "Product out of stock!" << endl;
                break;
            default:
                cout << "Invalid event" << endl;
                break;
        }
    }

    // 处理等待投币状态
    void handleWaitingForCoinState(Event event) {
        switch (event) {
            case Event::INSERT_COIN:
                coins++;
                currentState = VendingMachineState::COIN_INSERTED;
                cout << "Coin inserted successfully, current coins: " << coins << endl;
                break;
            case Event::RESET:
                coins = 0;
                selectedProduct = -1;
                currentState = VendingMachineState::IDLE;
                cout << "Reset to idle state" << endl;
                break;
            default:
                cout << "Invalid event" << endl;
                break;
        }
    }

    // 处理已投币状态
    void handleCoinInsertedState(Event event) {
        switch (event) {
            case Event::SELECT_PRODUCT:
                selectedProduct = 1; // 假设选择了商品1
                currentState = VendingMachineState::PRODUCT_SELECTED;
                cout << "Selected product " << selectedProduct << endl;
                break;
            case Event::INSERT_COIN:
                coins++;
                cout << "Continue inserting coins, current coins: " << coins << endl;
                break;
            case Event::RESET:
                coins = 0;
                selectedProduct = -1;
                currentState = VendingMachineState::IDLE;
                cout << "Reset to idle state" << endl;
                break;
            default:
                cout << "Invalid event" << endl;
                break;
        }
    }

    // 处理已选择商品状态
    void handleProductSelectedState(Event event) {
        switch (event) {
            case Event::DISPENSE_PRODUCT:
                if (coins >= 2) { // 假设商品价格是2个硬币
                    coins -= 2;
                    currentState = VendingMachineState::DISPENSING;
                    cout << "Start dispensing product " << selectedProduct << endl;
                } else {
                    cout << "Insufficient coins, need more coins" << endl;
                }
                break;
            case Event::INSERT_COIN:
                coins++;
                cout << "Continue inserting coins, current coins: " << coins << endl;
                break;
            case Event::RESET:
                coins = 0;
                selectedProduct = -1;
                currentState = VendingMachineState::IDLE;
                cout << "Reset to idle state" << endl;
                break;
            default:
                cout << "Invalid event" << endl;
                break;
        }
    }

    // 处理出货状态
    void handleDispensingState(Event event) {
        switch (event) {
            case Event::DISPENSE_PRODUCT:
                cout << "Dispensing completed!" << endl;
                selectedProduct = -1;
                if (coins > 0) {
                    currentState = VendingMachineState::COIN_INSERTED;
                    cout << "Remaining coins: " << coins << endl;
                } else {
                    currentState = VendingMachineState::IDLE;
                    cout << "Transaction completed" << endl;
                }
                break;
            case Event::RESET:
                coins = 0;
                selectedProduct = -1;
                currentState = VendingMachineState::IDLE;
                cout << "Reset to idle state" << endl;
                break;
            default:
                cout << "Invalid event" << endl;
                break;
        }
    }

    // 处理缺货状态
    void handleOutOfStockState(Event event) {
        switch (event) {
            case Event::RESET:
                hasStock = true;
                coins = 0;
                selectedProduct = -1;
                currentState = VendingMachineState::IDLE;
                cout << "Restocking completed, reset to idle state" << endl;
                break;
            default:
                cout << "Product out of stock, please wait for restocking" << endl;
                break;
        }
    }

    // 获取状态字符串
    string getStateString() {
        switch (currentState) {
            case VendingMachineState::IDLE: return "Idle";
            case VendingMachineState::WAITING_FOR_COIN: return "Waiting for coin";
            case VendingMachineState::COIN_INSERTED: return "Coin inserted";
            case VendingMachineState::PRODUCT_SELECTED: return "Product selected";
            case VendingMachineState::DISPENSING: return "Dispensing";
            case VendingMachineState::OUT_OF_STOCK: return "Out of stock";
            default: return "Unknown state";
        }
    }

    // 获取事件字符串
    string getEventString(Event event) {
        switch (event) {
            case Event::INSERT_COIN: return "Insert coin";
            case Event::SELECT_PRODUCT: return "Select product";
            case Event::DISPENSE_PRODUCT: return "Dispense product";
            case Event::OUT_OF_STOCK: return "Out of stock";
            case Event::RESET: return "Reset";
            default: return "Unknown event";
        }
    }

public:
    // 获取当前状态
    VendingMachineState getCurrentState() const {
        return currentState;
    }

    // 获取当前硬币数
    int getCoins() const {
        return coins;
    }
};

int main() {
    cout << "=== Vending Machine State Machine Example ===" << endl;
    
    VendingMachine machine;
    
    // 模拟状态机运行
    cout << "\n1. Initial state" << endl;
    machine.handleEvent(Event::INSERT_COIN);
    
    cout << "\n2. Select product" << endl;
    machine.handleEvent(Event::SELECT_PRODUCT);
    
    cout << "\n3. Continue inserting coins" << endl;
    machine.handleEvent(Event::INSERT_COIN);
    
    cout << "\n4. Dispense product" << endl;
    machine.handleEvent(Event::DISPENSE_PRODUCT);
    
    cout << "\n5. Complete dispensing" << endl;
    machine.handleEvent(Event::DISPENSE_PRODUCT);
    
    cout << "\n6. Simulate out of stock situation" << endl;
    machine.handleEvent(Event::OUT_OF_STOCK);
    
    cout << "\n7. Try to insert coin (out of stock state)" << endl;
    machine.handleEvent(Event::INSERT_COIN);
    
    cout << "\n8. Restock and reset" << endl;
    machine.handleEvent(Event::RESET);
    
    cout << "\n9. Insert coin again" << endl;
    machine.handleEvent(Event::INSERT_COIN);
    
    return 0;
}
