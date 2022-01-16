#ifndef MainWindow_H
#define MainWindow_H

#include <QMainWindow>
#include <QFrame>
#include <QPushButton>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Page indexes

enum {
    LOGIN_PAGE, HOME_PAGE, PRODUCT_DETAIL_PAGE, BUYER_PAGE, SELLER_PAGE, STAFF_PAGE, ADMIN_PAGE, ABOUT_US
};

enum {
    BUYER_INFO, BUYER_CART, BUYER_ORDERS
};

enum {
    SELLER_INFO, SELLER_SHOP, SELLER_ORDERS, SELLER_VOUCHERS, SELLER_STATS
};

enum {
    STAFF_INFO, STAFF_SHOP, STAFF_ORDERS, STAFF_EVENTS, STAFF_STATS
};

enum {
    ADMIN_INFO, ADMIN_STAFFS, ADMIN_SELLERS, ADMIN_EVENTS, ADMIN_STATS
};


#include "../Providers/LoginProvider/LoginProvider.h"
#include "../Providers/AccountProvider/AccountProvider.h"
#include "../Providers/ProductProvider/ProductProvider.h"
#include "../Providers/OrderProvider/OrderProvider.h"
#include "../Providers/VoucherProvider/VoucherProvider.h"

#include "ui_components/SubDialogs/mydialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    AccountProvider* _accountProvider;
    LoginProvider* _loginProvider;
    ProductProvider* _productProvider;
    OrderProvider* _orderProvider;
    VoucherProvider* _voucherProvider;
    Account* _account;
    string _role;

private:
    MyDialog* _cartEditDialog;
    MyDialog* _productDialog;
    MyDialog* _voucherDialog;
    MyDialog* _eventDialog;
    MyDialog* _rateOrderDialog;
    MyDialog* _checkoutDialog;
    MyDialog* _addStaffDialog;

private:
    Ui::MainWindow *ui;
    QFrame* _currentCategoryWidget;
    QFrame* _firstCategoryWidget;
    QString _currentCategoryName;
    QString _firstCategoryName;
    QPushButton* _currentBuyerSection;
    QPushButton* _currentSellerSection;
    QPushButton* _currentStaffSection;
    QPushButton* _currentAdminSection;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


////////////////////////////////////////////////////////////////////////////////
// Render and manipulation functions
////////////////////////////////////////////////////////////////////////////////
private slots:
///////////////////////////////////////////////////////////////////
// Common methods
    // Login
    void clearLoginInput();
    void loginFinished();
    void logout();

    // Account infos
    void renderAccountInfor();
    void setInfoReadonly(bool readonly);

    // Change account sections
    void changeAccountSection(QPushButton*& current, QPushButton*& next);

    // Reset page index
    void resetPages();

    // Clear data in scroll areas
    void clearScrollData();

    // Change password
    void changePassword(const string& oldPassword, const string& newPassword, const string& confirmPassword);
    void clearPasswordInput();

    // Wallet
    void accountDeposit();
    void accountWithdraw();
    void clearWalletInput();

    // Shop page
    void renderShopSidebar();
    void changeShopSidebar(QString category, QFrame* nextCategoryWidget);
    void renderProductGrid(QString searchInput = "", QString category = "love");
    void updateShopEvents();

    // Order list
    void renderOrderList(bool accepted);
    void renderOrderDetail(QString orderID);
    void removeOrder(QString ID);

    // Checkout
    void renderCheckoutDialog(QString productID = "", uint quantity = 0);
    void checkoutFinished(int errorCode, string data);

    // Statistics
    void renderStatistics();
    QFrame* renderStatsProduct(list<Product*> products);


///////////////////////////////////////////////////////////////////
// Methods for Buyer's Page
    // Cart
    void renderBuyerCart();
    void removeCartItem(QString orderID);
    void renderCartEditDialog(QString orderID);
    void cartEditDialogFinished(int quantity, string productID);

    // Orders
    void renderRateOrderDialog(QString orderID);
    void rateOrderFinished(string orderID, int rating);

///////////////////////////////////////////////////////////////////
// Methods for both Seller and Staff Page
    // Render product list and dialog, product detail
    void renderProductDetail(QString productID);
    QFrame* renderProductList(QLabel* productQuantityPlace);
    void renderEditProductDialog(QString productID);
    void productDialogFinished(bool isOk);
    void removeProduct(QString productID);

    // Orders
    void acceptOrder(QString orderID);

    // Render vouchers (or events)
    QFrame* renderVoucherList(QLabel* voucherQuantityPlace, bool isEvent);
    void renderVoucherDetail(QString productID);

///////////////////////////////////////////////////////////////////
// Methods for Seller's Page
    // Vouchers
    void renderSellerVouchers();
    void renderEditVoucherDialog(QString voucherID);
    void voucherDialogFinished(bool isOk);
    void removeVoucher(QString voucherID);

///////////////////////////////////////////////////////////////////
// Methods for Staff's Page
    // Events
    void renderStaffEvents();
    void renderEditEventDialog(QString eventID);
    void eventDialogFinished(int returnCode);
    void removeEvent(QString eventID);

///////////////////////////////////////////////////////////////////
// Methods for Admin's Page
    // Staffs
    void renderStaffList();
    void renderStaffDetail(QString username);
    void dismissStaff(QString username);
    void addStaffFinished(bool isOk);
    // Sellers
    void renderSellerList();
    void removeSeller(QString username);

////////////////////////////////////////////////////////////////////////////////
// Button events
////////////////////////////////////////////////////////////////////////////////
private slots:
    // Home page
    void on_login_clicked();
    void on_logo_clicked();
    void on_loginBtn_clicked();
    void on_backToShopBtn_clicked();
    void on_productRemoveBtn_clicked();
    void on_accountBtn_clicked();
    void on_searchBtn_clicked();
    void on_addCartBtn_clicked();
    void on_buyNowBtn_clicked();

    // Login page
    void on_signinBtn_clicked();
    void on_forgetPasswordBtn_clicked();
    void on_forgetPasswordConfirm_clicked();
    void on_forgetBtnBack_clicked();

private slots:
///////////////////////////////////////////////////////////////////
// Buyer's page
    // Info
    void on_buyerInfoBtn_clicked();
    void on_buyerEditInfo_clicked();
    void on_buyerSaveInfo_clicked();
    // Password
    void on_buyerInfoTab_currentChanged(int index);
    void on_buyerPasswordBtn_clicked();
    // Wallet
    void on_buyerDepositBtn_clicked();
    void on_buyerWithdrawBtn_clicked();
    // Cart
    void on_buyerCartBtn_clicked();
    void on_buyerCheckoutBtn_clicked();
    // Orders
    void on_buyerOrdersBtn_clicked();
    void on_buyerOrdersTab_currentChanged(int index);
    // Logout
    void on_buyerLogoutBtn_clicked();

///////////////////////////////////////////////////////////////////
// Seller's page
    // Info
    void on_sellerInfoBtn_clicked();
    void on_sellerEditInfo_clicked();
    void on_sellerSaveInfo_clicked();
    void on_sellerInfoTab_currentChanged(int index);
    // Password
    void on_sellerPasswordBtn_clicked();
    // Wallet
    void on_sellerDepositBtn_clicked();
    void on_sellerWithdrawBtn_clicked();
    // Shop
    void on_sellerShopBtn_clicked();
    void on_sellerAddProductBtn_clicked();
    // Orders
    void on_sellerOrdersBtn_clicked();
    void on_sellerOrdersTab_currentChanged(int index);
    // Vouchers
    void on_sellerVoucherBtn_clicked();
    void on_sellerAddVoucherBtn_clicked();
    // Statistics
    void on_sellerStatsBtn_clicked();
    // Logout
    void on_sellerLogoutBtn_clicked();

///////////////////////////////////////////////////////////////////
// Staff's page
    // Info
    void on_staffInfoBtn_clicked();
    void on_staffEditInfo_clicked();
    void on_staffSaveInfo_clicked();
    void on_staffInfoTab_currentChanged(int index);
    // Password
    void on_staffPasswordBtn_clicked();
    // Wallet
    void on_staffDepositBtn_clicked();
    void on_staffWithdrawBtn_clicked();
    // Shop
    void on_staffShopBtn_clicked();
    void on_staffAddProductBtn_clicked();
    void on_staffOrdersBtn_clicked();
    void on_staffOrdersTab_currentChanged(int index);
    // Events
    void on_staffEventsBtn_clicked();
    void on_staffAddEventBtn_clicked();
    // Statistics
    void on_staffStatsBtn_clicked();
    // Logout
    void on_staffLogoutBtn_clicked();

///////////////////////////////////////////////////////////////////
// Admin's page
    // Info
    void on_adminInfoBtn_clicked();
    void on_adminEditInfo_clicked();
    void on_adminSaveInfo_clicked();
    void on_adminInfoTab_currentChanged(int index);
    // Password
    void on_adminPasswordBtn_clicked();
    // Wallet
    void on_adminDepositBtn_clicked();
    void on_adminWithdrawBtn_clicked();
    // Staffs
    void on_adminStaffsBtn_clicked();
    void on_adminStaffPayBtn_clicked();
    void on_adminStaffAddBtn_clicked();
    // Sellers
    void on_adminSellersBtn_clicked();
    // Events
    void on_adminEventsBtn_clicked();
    // Statistics
    void on_adminStatsBtn_clicked();
    // Logout
    void on_adminLogoutBtn_clicked();

    void on_aboutUsBtn_clicked();

private:
    int showMessage(QString title, QString message, bool isError);
    int showOptionMessage(QString title, QString message, QString option1, QString option2);
    bool checkBuyerRole();
};

#endif // MainWindow_H
