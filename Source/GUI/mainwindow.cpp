#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGroupBox>
#include <QDebug>
#include <vector>
#include <QGridLayout>
#include <QFileDialog>
#include <QMessageBox>

#include "ui_components/ProductCard/productcard.h"
#include "ui_components/ListItem/CartItem/cartitem.h"
#include "ui_components/ListItem/OrderItem/orderitem.h"
#include "ui_components/ListItem/ProductItem/productitem.h"
#include "ui_components/ListItem/VoucherItem/voucheritem.h"
#include "ui_components/ListItem/StaffItem/staffitem.h"
#include "ui_components/ListItem/SellerItem/selleritem.h"
#include "ui_components/ListItem/StatisticItem/statisticitem.h"

#include "ui_components/SubDialogs/CartEditDialog/carteditdialog.h"
#include "ui_components/SubDialogs/ProductDialog/productdialog.h"
#include "ui_components/SubDialogs/VoucherDialog/voucherdialog.h"
#include "ui_components/SubDialogs/EventDialog/eventdialog.h"
#include "ui_components/SubDialogs/OrderDialog/rateorderdialog.h"
#include "ui_components/SubDialogs/CheckoutDialog/checkoutdialog.h"
#include "ui_components/SubDialogs/StaffDialog/addstaffdialog.h"
#include "ui_components/SubDialogs/OrderDialog/orderdetaildialog.h"
#include "ui_components/SubDialogs/StaffDialog/staffinfordialog.h"
#include "ui_components/SubDialogs/flexdialog.h"

#include "../Components/Account/Buyer.h"
#include "../Components/Account/Seller.h"
#include "../Components/Account/Staff.h"
#include "../Components/Account/Admin.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _productProvider = ProductProvider::getInstance();
    _loginProvider = LoginProvider::getInstance();
    _accountProvider = AccountProvider::getInstance();
    _orderProvider = OrderProvider::getInstance();
    _voucherProvider = VoucherProvider::getInstance();

    _account = nullptr;
    _role = "";

    ui->accountBtn->hide();
    ui->accountName->hide();
    ui->productRemoveBtn->hide();

    // Load the latest event of the shop
    this->updateShopEvents();

    // Set the page at Home
    ui->pages->setCurrentIndex(HOME_PAGE);

    // Load sidebar (for home page)
    _currentCategoryWidget = nullptr;
    _currentCategoryName = "";
    this->renderShopSidebar();

    // Load product list (for home page)
    this->renderProductGrid();

    QString selectedStyle = "background-color: #f2f2f2; color: #f27d85;";
    // Buyer's page
    _currentBuyerSection = ui->buyerInfoBtn;
    _currentBuyerSection->setStyleSheet(selectedStyle);
    // Seller's page
    _currentSellerSection = ui->sellerInfoBtn;
    _currentSellerSection->setStyleSheet(selectedStyle);
    // Staff's page
    _currentStaffSection = ui->staffInfoBtn;
    _currentStaffSection->setStyleSheet(selectedStyle);
    // Admin's page
    _currentAdminSection = ui->adminInfoBtn;
    _currentAdminSection->setStyleSheet(selectedStyle);

    // Product dialog
    _productDialog = new ProductDialog();
    connect(_productDialog, SIGNAL(added(bool)), this, SLOT(productDialogFinished(bool)));
    connect(_productDialog, SIGNAL(edited(bool)), this, SLOT(productDialogFinished(bool)));

    // Edit Cart Dialog
    _cartEditDialog = new CartEditDialog();
    connect(_cartEditDialog, SIGNAL(saved(int, string)), this, SLOT(cartEditDialogFinished(int, string)));

    // Rate Orders dialog
    _rateOrderDialog = new RateOrderDialog();
    connect(_rateOrderDialog, SIGNAL(saved(string, int)), this, SLOT(rateOrderFinished(string, int)));

    // Voucher dialog
    _voucherDialog = new VoucherDialog();
    connect(_voucherDialog, SIGNAL(saved(bool)), this, SLOT(voucherDialogFinished(bool)));

    // Event dialog
    _eventDialog = new EventDialog();
    connect(_eventDialog, SIGNAL(saved(int)), this, SLOT(eventDialogFinished(int)));

    // Checkout dialog
    _checkoutDialog = new CheckoutDialog();
    connect(_checkoutDialog, SIGNAL(checkoutFinished(int, string)), this, SLOT(checkoutFinished(int, string)));

    // Add staff dialog
    _addStaffDialog = new AddStaffDialog();
    connect(_addStaffDialog, SIGNAL(saved(bool)), this, SLOT(addStaffFinished(bool)));
}


MainWindow::~MainWindow()
{
    delete _productDialog;
    delete _cartEditDialog;
    delete _voucherDialog;
    delete _eventDialog;
    delete _rateOrderDialog;
    delete _checkoutDialog;
    delete _addStaffDialog;
    delete ui;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// Render and manipulation functions
////////////////////////////////////////////////////////////////////////////////////////////////

// Common methods

// -----------------------------------------------------------------------
// Login and logout

void MainWindow::clearLoginInput()
{
    ui->loginUsername->clear();
    ui->loginPassword->clear();

    ui->signinName->clear();
    ui->signinUsername->clear();
    ui->signInPassword->clear();
    ui->signinAddress->clear();
    ui->signinRole->setCurrentIndex(0);
    ui->signinPhoneNumber->clear();

    ui->forgetPasswordUsername->clear();
    ui->forgetPasswordPhonenumber->clear();
}

void MainWindow::loginFinished()
{
    ui->login->hide();
    ui->accountName->show();
    ui->accountBtn->show();

    if (_role == STAFF || _role == ADMIN) {
        ui->productRemoveBtn->show();
    } else {
        ui->productRemoveBtn->hide();
    }

    this->on_logo_clicked();
}


void MainWindow::logout()
{
    _account = nullptr;
    _role.clear();

    ui->accountBtn->hide();
    ui->accountName->hide();
    ui->login->show();

    this->on_logo_clicked();
}


// -----------------------------------------------------------------------
// Change account sections

void MainWindow::changeAccountSection(QPushButton *& current, QPushButton *& next)
{
    if (current == next) {
        return;
    }

    current->setStyleSheet("");
    current = next;
    current->setStyleSheet("background-color: #f2f2f2; color: #f27d85;");
}


// -----------------------------------------------------------------------
// Reset page index

void MainWindow::resetPages()
{
    if (_role == BUYER) {
        ui->buyerView->setCurrentIndex(0);
        ui->buyerInfoTab->setCurrentIndex(0);
        ui->buyerOrdersTab->setCurrentIndex(0);
        this->changeAccountSection(_currentBuyerSection, ui->buyerInfoBtn);
    } else if (_role == SELLER) {
        ui->sellerView->setCurrentIndex(0);
        ui->sellerInfoTab->setCurrentIndex(0);
        ui->sellerOrdersTab->setCurrentIndex(0);
        this->changeAccountSection(_currentSellerSection, ui->sellerInfoBtn);
    } else if (_role == STAFF) {
        ui->staffView->setCurrentIndex(0);
        ui->staffInfoTab->setCurrentIndex(0);
        ui->staffOrdersTab->setCurrentIndex(0);
        this->changeAccountSection(_currentStaffSection, ui->staffInfoBtn);
    } else {
        ui->adminView->setCurrentIndex(0);
        ui->adminInfoTab->setCurrentIndex(0);
        this->changeAccountSection(_currentAdminSection, ui->adminInfoBtn);
    }
}

// -----------------------------------------------------------------------
// Clear data in scroll areas

void MainWindow::clearScrollData()
{
    if (_role == BUYER) {
        QWidget* w1 = new QWidget();
        QWidget* w2 = new QWidget();
        ui->buyerCartScroll->setWidget(w1);
        ui->buyerOrdersScroll->setWidget(w2);
    } else {
        QWidget* w1 = new QWidget();
        QWidget* w2 = new QWidget();
        QWidget* w3 = new QWidget();
        QWidget* w4 = new QWidget();
        QWidget* w5 = new QWidget();
        if (_role == SELLER) {
            ui->sellerProductsScroll->setWidget(w1);
            ui->sellerOrdersScroll->setWidget(w2);
            ui->sellerVoucherScroll->setWidget(w3);
            ui->sellerHitProductsScroll->setWidget(w4);
            ui->sellerStaleProductsScroll->setWidget(w5);
        } else if (_role == STAFF) {
            ui->staffProductsScroll->setWidget(w1);
            ui->staffOrdersScroll->setWidget(w2);
            ui->staffEventScroll->setWidget(w3);
            ui->staffHitProductsScroll->setWidget(w4);
            ui->staffStaleProductsScroll->setWidget(w5);
        } else {
            ui->adminStaffScroll->setWidget(w1);
            ui->adminEventScroll->setWidget(w2);
            ui->adminHitProductsScroll->setWidget(w3);
            ui->adminStaleProductsScroll->setWidget(w4);
            delete w5;
        }
    }

    QWidget* w = new QWidget();
    ui->shopViewScroll->setWidget(w);
}


// -----------------------------------------------------------------------
// Change password

void MainWindow::changePassword(const string &oldPassword, const string &newPassword, const string &confirmPassword)
{
    if (newPassword != confirmPassword) {
        this->showMessage("Lỗi!", "Mật khẩu nhập lại không khớp", true);
        return;
    }

    bool isOk = _loginProvider->changePassword(_account->username(), oldPassword, newPassword);
    if (!isOk) {
        this->showMessage("Lỗi", "Mật khẩu hiện tại không đúng", true);
        return;
    }

    this->showMessage("Thành công", "Bạn đã thay đổi được mật khẩu", false);
    this->clearPasswordInput();
}

void MainWindow::clearPasswordInput() {
    if (_role == BUYER) {
        ui->buyerOldPassword->clear();
        ui->buyerNewPassword->clear();
        ui->buyerNewPasswordConfirm->clear();
    } else if (_role == SELLER) {
        ui->sellerOldPassword->clear();
        ui->sellerNewPassword->clear();
        ui->sellerNewPasswordConfirm->clear();
    } else if (_role == STAFF) {
        ui->staffOldPassword->clear();
        ui->staffNewPassword->clear();
        ui->staffNewPasswordConfirm->clear();
    } else {
        ui->adminOldPassword->clear();
        ui->adminNewPassword->clear();
        ui->adminNewPasswordConfirm->clear();
    }
}

// -----------------------------------------------------------------------
// Wallet

void MainWindow::accountDeposit() {
    QLineEdit* inputObj = nullptr;
    QLabel* showObj = nullptr;

    if (_role == BUYER) {
        inputObj = ui->buyerDepositInput;
        showObj = ui->buyerWalletBalance;
    } else if (_role == SELLER) {
        inputObj = ui->sellerDepositInput;
        showObj = ui->sellerWalletBalance;
    } else if (_role == STAFF) {
        inputObj = ui->staffDepositInput;
        showObj = ui->staffWalletBalance;
    } else {
        inputObj = ui->adminDepositInput;
        showObj = ui->adminWalletBalance;
    }

    bool check;
    ullong amount = inputObj->text().toULongLong(&check);

    if (!check) {
        this->showMessage("Lỗi!", "Thông tin nhập vào không hợp lệ", true);
        return;
    }

    _account->wallet()->deposit(amount);
    this->showMessage("Thành công!", "Bạn đã nạp tiền thành công", false);

    showObj->setText(QString::fromStdWString(vndPrice(_account->balance())));

    this->clearWalletInput();
}


void MainWindow::accountWithdraw() {
    QLineEdit* inputObj = nullptr;
    QLabel* showObj = nullptr;
    if (_role == BUYER) {
        inputObj = ui->buyerWithdrawInput;
        showObj = ui->buyerWalletBalance;
    } else if (_role == SELLER) {
        inputObj = ui->sellerWithdrawInput;
        showObj = ui->sellerWalletBalance;
    } else if (_role == STAFF) {
        inputObj = ui->staffWithdrawInput;
        showObj = ui->staffWalletBalance;
    } else {
        inputObj = ui->adminWithdrawInput;
        showObj = ui->adminWalletBalance;
    }

    bool check;
    ullong amount = inputObj->text().toULongLong(&check);

    if (!check) {
        this->showMessage("Lỗi!", "Thông tin nhập vào không hợp lệ", true);
        return;
    }

    check = _account->wallet()->withdraw(amount);

    if (!check) {
        this->showMessage("Lỗi!", "Số dư của bạn không đủ", true);
        return;
    }

    this->showMessage("Thành công!", "Bạn đã rút tiền thành công", false);
    showObj->setText(QString::fromStdWString(vndPrice(_account->balance())));

    this->clearWalletInput();
}


void MainWindow::clearWalletInput()
{
    if (_role == BUYER) {
        ui->buyerDepositInput->clear();
        ui->buyerWithdrawInput->clear();
    } else if (_role == SELLER) {
        ui->sellerDepositInput->clear();
        ui->sellerWithdrawInput->clear();
    } else if (_role == STAFF) {
        ui->staffDepositInput->clear();
        ui->staffWithdrawInput->clear();
    } else {
        ui->adminDepositInput->clear();
        ui->adminWithdrawInput->clear();
    }
}

// -----------------------------------------------------------------------
// Shop page

void MainWindow::renderShopSidebar()
{
    std::vector<QString> dicts = {"love.png", "birthday.png", "office.png", "congrats.png", "sad.png"};
    std::vector<QString> names = {"Hoa tình yêu", "Hoa sinh nhật", "Cây văn phòng", "Hoa chúc mừng", "Hoa chia buồn"};

    QVBoxLayout* sidebar = new QVBoxLayout();

    for (int i = 0; i < dicts.size(); i++) {
        MyFrame* button = new MyFrame();
        button->setStyleSheet("font-weight: bold; color: #333;");
        button->setWhatsThis(dicts[i].split(".")[0]);
        button->setCursor(Qt::PointingHandCursor);
        QVBoxLayout* layout = new QVBoxLayout();

        QLabel* image = new QLabel();
        image->setStyleSheet("border-image: url(:/images/categories/" + dicts[i] + ") 0 0 0 0 stretch stretch;");

        QLabel* name = new QLabel();
        name->setText(names[i]);

        layout->addWidget(image);
        layout->addWidget(name);

        layout->setAlignment(Qt::AlignCenter);
        layout->setStretch(0, 5);
        layout->setStretch(1, 1);

        button->setLayout(layout);

        if (i == 0) {
            button->setStyleSheet("background-color: #e8e8e8; font-weight: bold; color: #f27d85;");
            _currentCategoryWidget = button;
            _currentCategoryName = button->whatsThis();
            _firstCategoryWidget = button;
            _firstCategoryName = button->whatsThis();
        }
        sidebar->addWidget(button);

        // Change category event
        connect(button, SIGNAL(clicked(QString, QFrame*)), this, SLOT(changeShopSidebar(QString, QFrame*)));
    }

    ui->shopSidebar->setLayout(sidebar);
}


void MainWindow::changeShopSidebar(QString category, QFrame* nextCategoryWidget)
{
    if (_currentCategoryName == category) {
        return;
    }

    if (_currentCategoryWidget != nullptr) {
        _currentCategoryWidget->setStyleSheet("");
    }

    if (nextCategoryWidget != nullptr) {
        ui->searchInput->clear();

        nextCategoryWidget->setStyleSheet("background-color: #f2f2f2; font-weight: bold; color: #f27d85;");

        _currentCategoryName = category;
        _currentCategoryWidget = nextCategoryWidget;

        // Reload the product list
        this->renderProductGrid("", _currentCategoryName);
    }
    else {
        _currentCategoryWidget = nullptr;
        _currentCategoryName = "";
    }
}


void MainWindow::renderProductGrid(QString searchInput, QString category)
{
    int shopViewWidth = ui->shopViewScroll->width();

    list<Product*> products;

    if (searchInput.length() != 0) {
        wstring input = searchInput.toStdWString();
        products = _productProvider->getProductsByInput(input);
        this->changeShopSidebar("", nullptr);
    }
    else {
        products = _productProvider->getProductsByCategory(category.toStdString());
    }

    int nProduct = products.size();

    QFrame* container =  new QFrame;

    if (nProduct == 0) {
        QVBoxLayout* view = new QVBoxLayout;

        QLabel* image = new QLabel();
        QPixmap pix(":/images/empty/empty-result.png");
        image->setPixmap(pix.scaledToHeight(300, Qt::SmoothTransformation));
        image->setAlignment(Qt::AlignCenter);
        view->addWidget(image);

        QLabel* message = new QLabel();
        message->setStyleSheet("color: #333; font-size: 16px; font-weight: bold");
        message->setText("Rất tiếc, chúng tôi không thể tìm thấy sản phẩm liên quan");
        view->addWidget(message);

        view->setAlignment(Qt::AlignCenter);

        container->setLayout(view);
    }
    else {
        int nCol = 3;
        int nRow = ceil(1.0 * nProduct / 3);

        QGridLayout *mainGrid = new QGridLayout;
        mainGrid->setAlignment(Qt::AlignTop);
        mainGrid->setSpacing(16);
        mainGrid->setContentsMargins(16, 4, 8, 8);

        auto iter = products.begin();
        for (int i = 1; i <= nRow; i++) {
            for (int j = 1; j <= nCol; j++) {
                MyFrame* productCard = ProductCard::renderCard(int(shopViewWidth / nCol) - 28, 280, *iter);

                // Product detail Event
                connect(productCard, SIGNAL(clicked(QString)), this, SLOT(renderProductDetail(QString)));

                mainGrid->addWidget(productCard, i, j);

                iter++;
                if (iter == products.end()) break;
            }
            if (iter == products.end()) break;
        }

        // "Padding" to the grid if the number of product is less than nCol
        int temp = nCol - nProduct;
        for (; temp > 0; temp--) {
            MyFrame* tempFrame = new MyFrame();
            mainGrid->addWidget(tempFrame, 1, nCol - temp + 1);
        }

        container->setLayout(mainGrid);
    }

    ui->shopViewScroll->setWidgetResizable(true);
    ui->shopViewScroll->setWidget(container);
}

void MainWindow::updateShopEvents()
{
    Voucher* event = _voucherProvider->latestEvent();

    if (!event) {
        ui->searchInput->setPlaceholderText("Nhập thông tin bạn muốn tìm...");
        return;
    }

    QString name = QString::fromStdWString(event->name());
    QString percent = QString::number(event->salePercent());

    ui->searchInput->setPlaceholderText(name + "! Giảm " + percent + "% toàn bộ sản phẩm!");
}


// -----------------------------------------------------------------------
// Checkout page


// -----------------------------------------------------------------------
// Order list of account

void MainWindow::renderOrderList(bool accepted)
{
    string role = _account->role();
    int height = 120;

    bool isBuyer = role == BUYER;

    Member* member = (Member*)_account;
    list<string> orders = accepted ? member->acceptedOrders() : member->pendingOrders();
    int length = orders.size();

    QFrame* container = new QFrame;
    container->setStyleSheet("background-color: white; ");

    QVBoxLayout* list = new QVBoxLayout;
    list->setSpacing(8);

    if (length == 0) {
        // The order list is empty
        list->setAlignment(Qt::AlignCenter);

        // Image
        QLabel* image = new QLabel();
        QPixmap pix(":/images/empty/empty-order.png");
        image->setPixmap(pix.scaledToHeight(280, Qt::SmoothTransformation));
        image->setAlignment(Qt::AlignCenter);
        list->addWidget(image);

        // Text
        QString text = accepted ? "Không có đơn hàng nào được xác nhận" : "Không có đơn hàng nào đang chờ duyệt";
        QLabel* message = new QLabel();
        message->setText(text);
        message->setAlignment(Qt::AlignCenter);
        message->setStyleSheet("color: #333; font-weight: bold; font-size: 16px;");

        list->addWidget(message);
    }
    else {
        list->setAlignment(Qt::AlignTop);

        for (auto i = orders.begin(); i != orders.end(); i++) {
            Order* order = _orderProvider->getOrder(*i);
            OrderItemReturn result = OrderItem::renderItem(order, height, isBuyer);

            list->addWidget(result.item);
            connect(result.item, SIGNAL(clicked(QString)), this, SLOT(renderOrderDetail(QString)));

            // Button events
            // For Buyer: just use result.btn1
            if (isBuyer) {
            // Remove Button
                if (!accepted) {
                    connect(result.btn1, SIGNAL(buttonClicked(QString)), this, SLOT(removeOrder(QString)));
                }
                // Rate button
                else if (order->isRated() == false) {
                    connect(result.btn1, SIGNAL(buttonClicked(QString)), this, SLOT(renderRateOrderDialog(QString)));
                }
            }
            // For Seller (or Staff)
            else if (!accepted) {
                // Accept button
                connect(result.btn1, SIGNAL(buttonClicked(QString)), this, SLOT(acceptOrder(QString)));
                // Remove button
                connect(result.btn2, SIGNAL(buttonClicked(QString)), this, SLOT(removeOrder(QString)));
            }
        }
    }

    container->setLayout(list);

    if (role == BUYER) {
        ui->buyerOrdersScroll->setWidgetResizable(true);
        ui->buyerOrdersScroll->setWidget(container);
    } else if (role == SELLER) {
        ui->sellerOrdersScroll->setWidgetResizable(true);
        ui->sellerOrdersScroll->setWidget(container);
    } else if (role == STAFF) {
        ui->staffOrdersScroll->setWidgetResizable(true);
        ui->staffOrdersScroll->setWidget(container);
    }
}


void MainWindow::renderOrderDetail(QString ID)
{
    OrderDetailDialog* orderDialog = new OrderDetailDialog();
    orderDialog->loadInfor(ID.toStdString());
    orderDialog->setWindowTitle("Chi tiết đơn hàng");
    orderDialog->exec();
}

void MainWindow::removeOrder(QString orderID) {
    QString message = "";
    if (_role == BUYER) {
        message = "Bạn có chắc chắn muốn hủy đơn hàng này?";
    }
    else {
        message = "Bạn có chắc chắn muốn từ chối đơn hàng này?";
    }

    int reply = this->showOptionMessage("Xác nhận thao tác", message, "Có", "Không");
    if (reply == QMessageBox::StandardButton::No) return;

    string ID = orderID.toStdString();
    Order* order = _orderProvider->getOrder(ID);

    message = "";
    if (_role == BUYER) {
        message = "Bạn đã hủy đơn hàng có mã " + orderID;
        if (order->payWithWallet()) {
            message += "\nSố tiền đã được hoàn lại vào ví Bily";
        }
    }
    else {
        message = "Bạn đã từ chối đơn hàng có mã " + orderID;
    }

    _orderProvider->removeOrder(ID);
    this->showMessage("Thành công", message, false);
    this->renderOrderList(false);
}

void MainWindow::renderCheckoutDialog(QString productID, uint quantity)
{
    _checkoutDialog->loadInfor(productID.toStdString(), quantity);
    _checkoutDialog->exec();
}

void MainWindow::checkoutFinished(int errorCode, string data)
{
    switch (errorCode) {
    case 0:
        this->showMessage("Tuyệt!", "Bạn đã đặt hàng thành công", false);
        if (data == "cart") {
            this->renderBuyerCart();
        }
        else {
            this->on_logo_clicked();
        }
        return;
    case -1:
        this->showMessage("Lỗi!", "Số dư trong ví của bạn không đủ", false);
        return;
    case -2: {
        Product* product = _productProvider->getProduct(data);
        QString name = QString::fromStdWString(product->name());
        this->showMessage("Lỗi!", "Sản phẩm " + name + " đã không còn đủ hàng" , false);
        return;
    }
    default:
        return;
    }
}

// -----------------------------------------------------------------------
// Statistics

QFrame* MainWindow::renderStatsProduct(list<Product*> products) {
    int height = 83;

    QFrame* container = new QFrame;
    container->setStyleSheet("background-color: white; ");

    QVBoxLayout* list = new QVBoxLayout;
    list->setSpacing(6);
    if (products.empty()) {
        list->setAlignment(Qt::AlignCenter);

        QLabel* image = new QLabel();
        QPixmap pix(":/images/empty/empty-product.png");
        image->setPixmap(pix.scaledToHeight(120, Qt::SmoothTransformation));
        image->setAlignment(Qt::AlignCenter);
        list->addWidget(image);

        QLabel* message = new QLabel();
        message->setStyleSheet("color: #333; font-size: 14px; font-weight: bold");
        message->setText("Danh sách rỗng");
        message->setAlignment(Qt::AlignCenter);
        list->addWidget(message);
    }
    else {
        list->setAlignment(Qt::AlignTop);
        for (auto product: products) {
            QFrame* item = StatisticItem::renderItem(product, height);
            list->addWidget(item);
        }
    }

    container->setLayout(list);
    return container;
}

void MainWindow::renderStatistics()
{
    if (_role == BUYER) return;

    Statistics result = _accountProvider->getStatistics(_account);
    QString totalRevenue = QString::fromStdWString(vndPrice(result.totalRevenue));
    QString avgRating = QString::number(result.avgRating, 'g', 2);
    QString soldQuantity = QString::number(result.soldQuantity);
    QString shopQuantity = QString::number(result.shopQuantity);
    QFrame* hitContainer = this->renderStatsProduct(result.hitProducts);
    QFrame* staleContainer = this->renderStatsProduct(result.staleProducts);

    if (_role == SELLER) {
        ui->sellerRevenue->setText(totalRevenue);
        ui->sellerAvgRating->setText(avgRating);
        ui->sellerSoldQuantity->setText(soldQuantity);
        ui->sellerPostedQuantity->setText(shopQuantity);

        ui->sellerHitProductsScroll->setWidgetResizable(true);
        ui->sellerHitProductsScroll->setWidget(hitContainer);

        ui->sellerStaleProductsScroll->setWidgetResizable(true);
        ui->sellerStaleProductsScroll->setWidget(staleContainer);
    }
    else if (_role == STAFF) {
        ui->staffRevenue->setText(totalRevenue);
        ui->staffAvgRating->setText(avgRating);
        ui->staffSoldQuantity->setText(soldQuantity);
        ui->staffPostedQuantity->setText(shopQuantity);

        ui->staffHitProductsScroll->setWidgetResizable(true);
        ui->staffHitProductsScroll->setWidget(hitContainer);

        ui->staffStaleProductsScroll->setWidgetResizable(true);
        ui->staffStaleProductsScroll->setWidget(staleContainer);
    }
    else if (_role == ADMIN) {
        QString shopRevenue = QString::fromStdWString(vndPrice(result.shopRevenue));

        ullong profit = result.profitFromSeller < 0 ? -result.profitFromSeller : result.profitFromSeller;
        QString profitFromSeller = QString::fromStdWString(vndPrice(profit));
        if (result.profitFromSeller < 0) profitFromSeller = "-" + profitFromSeller;

        ui->adminTotalRevenue->setText(totalRevenue);
        ui->adminShopRevenue->setText(shopRevenue);
        ui->adminProfitFromSeller->setText(profitFromSeller);
        ui->adminAvgRating->setText(avgRating);
        ui->adminSoldQuantity->setText(soldQuantity);
        ui->adminShopQuantity->setText(shopQuantity);

        ui->adminHitProductsScroll->setWidgetResizable(true);
        ui->adminHitProductsScroll->setWidget(hitContainer);

        ui->adminStaleProductsScroll->setWidgetResizable(true);
        ui->adminStaleProductsScroll->setWidget(staleContainer);
    }

}

// -----------------------------------------------------------------------
// Account's infor

void MainWindow::renderAccountInfor()
{
    QStringList nameWords = QString::fromStdWString(_account->name()).split(" ");
    QString lastNme = nameWords[nameWords.size() - 1];
    ui->accountName->setText(lastNme);

    QString name = QString::fromStdWString(_account->name());
    QString username = QString::fromStdString(_account->username());
    QString phoneNumber = QString::fromStdString(_account->phoneNumber());
    QString address = QString::fromStdWString(_account->address());
    QString balance = QString::fromStdWString(vndPrice(_account->balance()));

    if (_role == SELLER) {
        ui->sellerName->setText(name);
        ui->sellerUsername->setText(username);
        ui->sellerPhoneNumber->setText(phoneNumber);
        ui->sellerAddress->setText(address);
        ui->sellerWalletBalance->setText(balance);
    }
    else if (_role == STAFF) {
        ui->staffName->setText(name);
        ui->staffUsername->setText(username);
        ui->staffPhoneNumber->setText(phoneNumber);
        ui->staffAddress->setText(address);
        ui->staffWalletBalance->setText(balance);
    }
    else if (_role == BUYER) {
        ui->buyerName->setText(name);
        ui->buyerUsername->setText(username);
        ui->buyerPhoneNumber->setText(phoneNumber);
        ui->buyerAddress->setText(address);
        ui->buyerWalletBalance->setText(balance);
    }
    else {
        ui->adminName->setText(name);
        ui->adminUsername->setText(username);
        ui->adminPhoneNumber->setText(phoneNumber);
        ui->adminAddress->setText(address);
        ui->adminWalletBalance->setText(balance);
    }
}


void MainWindow::setInfoReadonly(bool readonly)
{
    QString style = readonly ? "border: none;" : "";

    if (_role == BUYER) {
        ui->buyerName->setReadOnly(readonly);
        ui->buyerName->setStyleSheet(style);
        ui->buyerPhoneNumber->setReadOnly(readonly);
        ui->buyerPhoneNumber->setStyleSheet(style);
        ui->buyerAddress->setReadOnly(readonly);
        ui->buyerAddress->setStyleSheet(style);
    } else if (_role == SELLER) {
        ui->sellerName->setReadOnly(readonly);
        ui->sellerName->setStyleSheet(style);
        ui->sellerPhoneNumber->setReadOnly(readonly);
        ui->sellerPhoneNumber->setStyleSheet(style);
        ui->sellerAddress->setReadOnly(readonly);
        ui->sellerAddress->setStyleSheet(style);
    } else if (_role == STAFF) {
        ui->staffName->setReadOnly(readonly);
        ui->staffName->setStyleSheet(style);
        ui->staffPhoneNumber->setReadOnly(readonly);
        ui->staffPhoneNumber->setStyleSheet(style);
        ui->staffAddress->setReadOnly(readonly);
        ui->staffAddress->setStyleSheet(style);
    } else {
        ui->adminName->setReadOnly(readonly);
        ui->adminName->setStyleSheet(style);
        ui->adminPhoneNumber->setReadOnly(readonly);
        ui->adminPhoneNumber->setStyleSheet(style);
        ui->adminAddress->setReadOnly(readonly);
        ui->adminAddress->setStyleSheet(style);
    }
}

///////////////////////////////////////////////////////////////////
// Methods for Buyer

// Cart

void MainWindow::renderBuyerCart()
{
    int height = 90;
    Buyer* buyer = (Buyer*)_account;

    list<pair<string, uint>> items = buyer->cart()->items();

    // Total quantity in cart
    ui->buyerCartQuantity->setText(QString::number(buyer->cart()->totalQuantity()));

    // Items
    QFrame* container = new QFrame;
    container->setStyleSheet("background-color: white; ");
    QVBoxLayout* list = new QVBoxLayout;

    if (items.empty()) {
        QLabel* image = new QLabel();
        QPixmap pix(":/images/empty/empty-cart.png");
        image->setPixmap(pix.scaledToHeight(280, Qt::SmoothTransformation));
        image->setAlignment(Qt::AlignCenter);
        list->addWidget(image);

        QLabel* message = new QLabel();
        message->setStyleSheet("color: #333; font-size: 16px; font-weight: bold");
        message->setText("Giỏ hàng của bạn đang trống");
        message->setAlignment(Qt::AlignCenter);
        list->addWidget(message);

        QPushButton* button = new QPushButton();
        button->setText("Đi chọn hoa!");
        button->setCursor(Qt::PointingHandCursor);
        button->setStyleSheet("margin-top: 8px; margin-left: 100px; margin-right: 100px; border-radius: 15px; padding: 6px 8px; color: white; font-size: 16px; font-weight: bold; background-color: #66b7af; border: none;");
        list->addWidget(button);

        connect(button, SIGNAL(clicked()), this, SLOT(on_logo_clicked()));

        list->setAlignment(Qt::AlignCenter);
    }
    else {
        list->setAlignment(Qt::AlignTop);
        list->setSpacing(8);

        for (auto i = items.begin(); i != items.end(); i++) {
            string ID = i->first;
            uint quantity = i->second;
            Product* product = _productProvider->getProduct(ID);

            CartItemReturn result = CartItem::renderItem(product, quantity, height);

            list->addWidget(result.item);

            connect(result.editBtn, SIGNAL(buttonClicked(QString)), this, SLOT(renderCartEditDialog(QString)));
            connect(result.removeBtn, SIGNAL(buttonClicked(QString)), this, SLOT(removeCartItem(QString)));
        }
    }

    container->setLayout(list);

    ui->buyerCartScroll->setWidgetResizable(true);
    ui->buyerCartScroll->setWidget(container);
}


void MainWindow::removeCartItem(QString ID) {
    QString message = "Bạn có chắc chắn muốn xóa sản phẩm này khỏi giỏ hàng?";
    int reply = this->showOptionMessage("Xác nhận thao tác", message, "Có", "Không");

    if (reply == QMessageBox::StandardButton::No) return;

    ((Buyer*)_account)->cart()->removeItem(ID.toStdString());
    this->renderBuyerCart();
}


void MainWindow::renderCartEditDialog(QString ID)
{
    _cartEditDialog->loadInfor(ID.toStdString());
    _cartEditDialog->exec();
}


void MainWindow::cartEditDialogFinished(int quantity, string productID)
{
    if (quantity == -1) {
        this->showMessage("Lỗi!", "Thông tin nhập vào không hợp lệ", true);
        return;
    }

    if (quantity == -2) {
        this->showMessage("Lỗi!", "Số lượng sản phẩm có sẵn không đủ", true);
        return;
    }

    ((Buyer*)_account)->cart()->changeQuantity(productID, quantity);
    this->renderBuyerCart();
}

// Orders

void MainWindow::renderRateOrderDialog(QString ID)
{
    _rateOrderDialog->setObjID(ID.toStdString());
    _rateOrderDialog->exec();
}

void MainWindow::rateOrderFinished(string orderID, int rating)
{
    _orderProvider->rateOrder(orderID, rating);
    this->showMessage("Tuyệt!", "Bạn đã đánh giá đơn hàng thành công", false);
    this->renderOrderList(true);
}

///////////////////////////////////////////////////////////////////
// Methods for both Seller and Staff

// Products

void MainWindow::renderProductDetail(QString productID)
{
    Product* product = _productProvider->getProduct(productID.toStdString());

    // Product's id
    ui->productID->setText("Mã SP: " + productID);
    ui->productID->setWhatsThis(productID);
    ui->productRemoveBtn->setWhatsThis(productID);

    // Product's image
    int width = ui->productImage->width() - 10;
    int height = ui->productImage->height() - 10;
    QPixmap pix(QString::fromStdString(product->imageDir()));
    ui->productImage->setPixmap(pix.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->productImage->setAlignment(Qt::AlignCenter);

    Seller* seller = (Seller*)_accountProvider->getAccount(product->sellerUsername());

    // Product's info
    ui->productName->setText(QString::fromStdWString(product->name()));
    ui->productPrice->setText(QString::fromStdWString(vndPrice(product->salePrice())));
    if (product->salePrice() != product->originalPrice()) {
        ui->productOriginalPrice->setText(QString::fromStdWString(vndPrice(product->originalPrice())));
    } else {
        ui->productOriginalPrice->clear();
    }
    ui->productSeller->setText(QString::fromStdWString(seller->name()));
    ui->productAddress->setText(QString::fromStdWString(seller->address()));
    ui->productDesc->setText(QString::fromStdWString(product->description()));
    ui->productStock->setText(QString::number(product->stock()));
    ui->productRate->setText(QString::number(product->avgRating(), 'g', 2));
    ui->productSold->setText(QString::number(product->sold()));
    ui->productBuyQuantity->setText("1");

    // Product's category
    QString category;
    switch (product->category()) {
    case 0:
        category = "Hoa tình yêu";
        break;
    case 1:
        category = "Hoa sinh nhật";
        break;
    case 2:
        category = "Cây văn phòng";
        break;
    case 3:
        category = "Hoa chúc mừng";
        break;
    case 4:
        category = "Hoa chia buồn";
        break;
    default:
        break;
    }
    ui->productCategory->setText(category);

    // Style for special product
    if (seller->role() == STAFF) {
        QString name = QString::fromStdWString(seller->name()) + " (Nhân viên)";
        ui->productSeller->setText(name);
        ui->productSeller->setStyleSheet("color: #006d77; font-weight: bold");
    }
    else {
        ui->productSeller->setStyleSheet("");
    }

    // Set page
    ui->pages->setCurrentIndex(PRODUCT_DETAIL_PAGE);
}


QFrame *MainWindow::renderProductList(QLabel* productQuantityPlace)
{
    int height = 100;

    list<string> productIDs = ((Seller*)_account)->products();

    productQuantityPlace->setText(QString::number(productIDs.size()));

    QFrame* container = new QFrame;
    container->setStyleSheet("background-color: white; ");
    QVBoxLayout* list = new QVBoxLayout;

    if (productIDs.empty()) {
        list->setAlignment(Qt::AlignCenter);

        QLabel* image = new QLabel();
        QPixmap pix(":/images/empty/empty-product.png");
        image->setPixmap(pix.scaledToHeight(250, Qt::SmoothTransformation));
        image->setAlignment(Qt::AlignCenter);
        list->addWidget(image);

        QLabel* message = new QLabel();
        message->setStyleSheet("color: #333; font-size: 16px; font-weight: bold");
        message->setText("Bạn chưa đăng sản phẩm nào.");
        message->setAlignment(Qt::AlignCenter);

        list->addWidget(message);
    }
    else {
        list->setAlignment(Qt::AlignTop);
        list->setSpacing(8);

        for (auto ID = productIDs.begin(); ID != productIDs.end(); ID++) {
            Product* product = _productProvider->getProduct(*ID);

            ProductItemReturn result = ProductItem::renderItem(product, height);

            list->addWidget(result.item);

            // Edit and remove button clicked events
            connect(result.editBtn, SIGNAL(buttonClicked(QString)), this, SLOT(renderEditProductDialog(QString)));
            connect(result.removeBtn, SIGNAL(buttonClicked(QString)), this, SLOT(removeProduct(QString)));
        }
    }

    container->setLayout(list);

    return container;
}


void MainWindow::renderEditProductDialog(QString productID)
{
    string ID = productID.toStdString();
    ((FlexDialog*)_productDialog)->setType(true);
    _productDialog->loadInfor(ID);
    _productDialog->exec();
}


void MainWindow::productDialogFinished(bool isOk) {
    if (!isOk) {
        this->showMessage("Lỗi!", "Vui lòng cung cấp đầy đủ thông tin!", true);
    }
    else {
        // Tải lại danh sách sản phẩm
        if (_role == SELLER) {
            this->on_sellerShopBtn_clicked();
        }
        else {
            this->on_staffShopBtn_clicked();
        }
    }
}


void MainWindow::removeProduct(QString productID)
{
    QString message = "Bạn có chắc chắn muốn xóa sản phẩm này?";
    int reply = this->showOptionMessage("Xác nhận thao tác", message, "Có", "Không");

    if (reply == QMessageBox::StandardButton::No) return;

    string ID = productID.toStdString();
    _productProvider->removeProduct(ID);

    message = "Bạn đã xóa sản phẩm có mã là " + productID;
    message += ".\nNhững đơn hàng liên quan cũng đã bị xóa bỏ.";
    reply = this->showMessage("Thành công!", message, false);

    if (reply == QMessageBox::Ok) {
        // Tải lại danh sách sản phẩm
        if (_role == SELLER) {
            this->on_sellerShopBtn_clicked();
        }
        else {
            this->on_staffShopBtn_clicked();
        }
    }
}

// -----------------------------------------------------------------------
// Orders

void MainWindow::acceptOrder(QString orderID)
{
    ullong baseSalary = 0;
    if (_role == STAFF) {
        baseSalary = ((Staff*)_account)->baseSalary();
    }

    _orderProvider->acceptOrder(orderID.toStdString());

    QString message = "Bạn đã duyệt đơn hàng có mã " + orderID;
    if (_role == SELLER) {
        message += "\nSố tiền đã được cộng vào Ví Bily   ";
    } else {
        message += "\nSố tiền đã được cộng vào doanh thu cửa hàng";
        baseSalary = ((Staff*)_account)->baseSalary() - baseSalary;
    }
    this->showMessage("Thành công", message, false);

    if (baseSalary > 0) {
        message = "Xin chúc mừng! Bạn đã được tăng lương!";
        this->showMessage("Tuyệt vời!", message, false);
    }

    this->renderOrderList(false);
}


// -----------------------------------------------------------------------
// Vouchers (or Events)

QFrame* MainWindow::renderVoucherList(QLabel* voucherQuantityPlace, bool isEvent) {
    int height = 90;
    list<string> voucherIDs;

    if (_role == ADMIN) {
        voucherIDs = _voucherProvider->events();
    } else {
        voucherIDs = ((Seller*)_account)->vouchers();
    }

    voucherQuantityPlace->setText(QString::number(voucherIDs.size()));
    voucherQuantityPlace->setStyleSheet("color: #006d77; font-weight: bold; font-size: 18px;");

    QFrame* container = new QFrame;
    container->setStyleSheet("background-color: white; ");
    QVBoxLayout* list = new QVBoxLayout;

    if (voucherIDs.empty()) {
        list->setAlignment(Qt::AlignCenter);

        QLabel* image = new QLabel();
        QPixmap pix(":/images/empty/empty-event.png");
        image->setPixmap(pix.scaledToHeight(300, Qt::SmoothTransformation));
        image->setAlignment(Qt::AlignCenter);
        list->addWidget(image);

        QLabel* message = new QLabel();
        message->setStyleSheet("color: #333; font-size: 16px; font-weight: bold");
        message->setAlignment(Qt::AlignCenter);

        if (_role == ADMIN) {
            message->setText("Cửa hàng chưa có sự kiện nào");
        } else {
            QString text = isEvent ? "Sự kiện nào" : "Voucher nào";
            message->setText("Bạn chưa tạo " + text);
        }

        list->addWidget(message);
    }
    else {
        list->setAlignment(Qt::AlignTop);
        list->setSpacing(8);

        for (auto ID = voucherIDs.begin(); ID != voucherIDs.end(); ID++) {
            Voucher* voucher = _voucherProvider->getVoucher(*ID);

            VoucherItemReturn result = (_role == ADMIN) ? VoucherItem::renderAdminItem(voucher, height)
                                                        : VoucherItem::renderNormalItem(voucher, height, isEvent);

            list->addWidget(result.item);

            // Only remove button for admin
            if (_role == ADMIN) {
                connect(result.removeBtn, SIGNAL(buttonClicked(QString)), this, SLOT(removeEvent(QString)));
            }
            // Edit and remove button clicked events for Staff and Seller
            else if (isEvent) {
                connect(result.editBtn, SIGNAL(buttonClicked(QString)), this, SLOT(renderEditEventDialog(QString)));
                connect(result.removeBtn, SIGNAL(buttonClicked(QString)), this, SLOT(removeEvent(QString)));
            } else {
                connect(result.editBtn, SIGNAL(buttonClicked(QString)), this, SLOT(renderEditVoucherDialog(QString)));
                connect(result.removeBtn, SIGNAL(buttonClicked(QString)), this, SLOT(removeVoucher(QString)));
            }
        }
    }

    container->setLayout(list);

    return container;
}

void MainWindow::renderVoucherDetail(QString productID) {

}

///////////////////////////////////////////////////////////////////
// Methods for Seller

// Vouchers

void MainWindow::renderSellerVouchers()
{
    QFrame* container = this->renderVoucherList(ui->sellerVoucherQuantity, false);
    ui->sellerVoucherScroll->setWidgetResizable(true);
    ui->sellerVoucherScroll->setWidget(container);
}


void MainWindow::renderEditVoucherDialog(QString voucherID)
{
    ((FlexDialog*)_voucherDialog)->setType(true);
    _voucherDialog->loadInfor(voucherID.toStdString());
    _voucherDialog->exec();
}

void MainWindow::voucherDialogFinished(bool isOk)
{
    if (!isOk) {
        this->showMessage("Lỗi!", "Thông tin nhập vào không hợp lệ", true);
        return;
    }

    this->on_sellerVoucherBtn_clicked();
}


void MainWindow::removeVoucher(QString voucherID)
{
    QString message = "Bạn có chắc chắn muốn xóa voucher này?";
    int reply = this->showOptionMessage("Xác nhận thao tác", message, "Có", "Không");

    if (reply == QMessageBox::StandardButton::No) return;

    _voucherProvider->removeVoucher(voucherID.toStdString());

    this->showMessage("Thành công!", "Bạn đã xóa Voucher có mã " + voucherID, false);
    this->on_sellerVoucherBtn_clicked();
}


///////////////////////////////////////////////////////////////////
// Methods for Staff

void MainWindow::renderStaffEvents() {
    QFrame* container = this->renderVoucherList(ui->staffEventQuantity, true);
    ui->staffEventScroll->setWidgetResizable(true);
    ui->staffEventScroll->setWidget(container);
}


void MainWindow::renderEditEventDialog(QString voucherID) {
    ((FlexDialog*)_eventDialog)->setType(true);
    _eventDialog->loadInfor(voucherID.toStdString());
    _eventDialog->exec();
}

void MainWindow::eventDialogFinished(int returnCode) {
    if (returnCode == -1) {
        this->showMessage("Lỗi!", "Thông tin nhập vào không hợp lệ", true);
        return;
    }

    this->on_staffEventsBtn_clicked();
    this->updateShopEvents();
}


void MainWindow::removeEvent(QString eventID) {
    QString message = "Bạn có chắc chắn muốn xóa sự kiện này?";
    int reply = this->showOptionMessage("Xác nhận thao tác", message, "Có", "Không");

    if (reply == QMessageBox::StandardButton::No) return;

    _voucherProvider->removeVoucher(eventID.toStdString());

    this->showMessage("Thành công!", "Bạn đã xóa Sự kiện có mã " + eventID, false);
    if (_role == STAFF) {
        this->on_staffEventsBtn_clicked();
    } else {
        this->on_adminEventsBtn_clicked();
    }

    this->updateShopEvents();
}

///////////////////////////////////////////////////////////////////
// Methods for Admin

void MainWindow::renderStaffList()
{
    list<string> staffs = ((Admin*)_account)->staffs();
    ui->adminStaffQuantity->setText(QString::number(staffs.size()));
    int height = 45;

    QFrame* container = new QFrame();
    QVBoxLayout* list = new QVBoxLayout();

    if (staffs.empty()) {
        list->setAlignment(Qt::AlignCenter);

        QLabel* image = new QLabel();
        QPixmap pix(":/images/empty/nobody.png");
        image->setPixmap(pix.scaledToHeight(300, Qt::SmoothTransformation));
        image->setAlignment(Qt::AlignCenter);
        list->addWidget(image);

        QLabel* message = new QLabel();
        message->setStyleSheet("color: #333; font-size: 16px; font-weight: bold");
        message->setText("Cửa hàng hiện không có nhân viên nào");
        message->setAlignment(Qt::AlignCenter);

        list->addWidget(message);
    }
    else {
        list->setAlignment(Qt::AlignTop);
        list->setSpacing(10);

        for (auto i = staffs.begin(); i != staffs.end(); i++) {
            Staff* staff = (Staff*)_accountProvider->getAccount(*i);
            StaffItemReturn result = StaffItem::renderItem(staff, height);

            list->addWidget(result.item);

            // connect event
            connect(result.item, SIGNAL(clicked(QString)), this, SLOT(renderStaffDetail(QString)));
            connect(result.btn, SIGNAL(buttonClicked(QString)), this, SLOT(dismissStaff(QString)));
        }
    }

    container->setLayout(list);
    ui->adminStaffScroll->setWidgetResizable(true);
    ui->adminStaffScroll->setWidget(container);
}

void MainWindow::renderSellerList()
{
    list <Account*> sellers = _accountProvider->getAccounts(SELLER);
    ui->adminSellerQuantity->setText(QString::number(sellers.size()));
    int height = 45;

    QFrame* container = new QFrame();
    QVBoxLayout* list = new QVBoxLayout();

    if (sellers.empty()) {
        list->setAlignment(Qt::AlignCenter);

        QLabel* image = new QLabel();
        QPixmap pix(":/images/empty/nobody.png");
        image->setPixmap(pix.scaledToHeight(300, Qt::SmoothTransformation));
        image->setAlignment(Qt::AlignCenter);
        list->addWidget(image);

        QLabel* message = new QLabel();
        message->setStyleSheet("color: #333; font-size: 16px; font-weight: bold");
        message->setText("Hệ thống chưa có người bán nào tham gia");
        message->setAlignment(Qt::AlignCenter);

        list->addWidget(message);
    }
    else {
        list->setAlignment(Qt::AlignTop);
        list->setSpacing(10);

        for (auto i = sellers.begin(); i != sellers.end(); i++) {
            SellerItemReturn result = SellerItem::renderItem(*i, height);

            list->addWidget(result.item);

            // connect event
            connect(result.btn, SIGNAL(buttonClicked(QString)), this, SLOT(removeSeller(QString)));
        }
    }

    container->setLayout(list);
    ui->adminSellerScroll->setWidgetResizable(true);
    ui->adminSellerScroll->setWidget(container);
}

void MainWindow::removeSeller(QString username)
{
    QString name = QString::fromStdWString(_accountProvider->getAccount(username.toStdString())->name());

    QString message = "Bạn có chắc chắn muốn hủy hợp đồng với " + name + "?";

    int reply = this->showOptionMessage("Xác nhận thao tác", message, "Có", "Không");
    if (reply == QMessageBox::StandardButton::Ok) {
        _accountProvider->removeAccount(username.toStdString(), false);
        this->showMessage("Thành công", "Bạn đã hủy hợp đồng thành công", false);
        this->renderSellerList();
    }
}

void MainWindow::renderStaffDetail(QString username)
{
    StaffInforDialog* staffDialog = new StaffInforDialog();
    staffDialog->loadInfor(username.toStdString());
    staffDialog->exec();
}

void MainWindow::dismissStaff(QString username)
{
    QString name = QString::fromStdWString(_accountProvider->getAccount(username.toStdString())->name());

    QString message = "Bạn có chắc chắn muốn sa thải nhân viên " + name + "?";

    int reply = this->showOptionMessage("Xác nhận thao tác", message, "Có", "Không");
    if (reply == QMessageBox::StandardButton::Ok) {
        ((Admin*)_account)->dismissStaff(username.toStdString());
        this->showMessage("Thành công", "Bạn đã sa thải nhân viên " + name, false);
        this->updateShopEvents();
        this->renderStaffList();
    }
}

void MainWindow::addStaffFinished(bool isOk)
{
    if (isOk) {
        this->showMessage("Thành công", "Tuyệt vời! Cửa hàng đã có thêm nhân viên mới", false);
        this->renderStaffList();
    } else {
        this->showMessage("Lỗi", "Vui lòng cung cấp đầy đủ thông tin", true);
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// Button events
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
// Home pages

void MainWindow::on_logo_clicked()
{
    this->clearScrollData();
    ui->searchInput->clear();

    ui->pages->setCurrentIndex(HOME_PAGE);

    this->renderProductGrid();
    this->changeShopSidebar(_firstCategoryName, _firstCategoryWidget);
}


void MainWindow::on_login_clicked()
{
    ui->searchInput->clear();
    this->clearLoginInput();
    ui->pages->setCurrentIndex(LOGIN_PAGE);
}


void MainWindow::on_searchBtn_clicked()
{
    QString input = ui->searchInput->text();
    if (input.length() == 0) {
        this->showMessage("Lỗi!", "Vui lòng nhập thông tin tìm kiếm", true);
        return;
    }

    if (ui->pages->currentIndex() != HOME_PAGE) {
        ui->pages->setCurrentIndex(HOME_PAGE);
    }

    this->renderProductGrid(input);
}


void MainWindow::on_addCartBtn_clicked()
{
    if (!this->checkBuyerRole()) {
        return;
    }

    string ID = ui->productID->whatsThis().toStdString();
    uint stock = ui->productStock->text().toUInt();

    bool check = false;
    uint quantity = ui->productBuyQuantity->text().toUInt(&check);

    if (!check) {
        this->showMessage("Lỗi!", "Thông tin nhập vào không hợp lệ", true);
        return;
    }

    if (quantity > stock) {
        this->showMessage("Lỗi!", "Số lượng sản phẩm có sẵn không đủ", true);
        return;
    }

    ((Buyer*)_account)->cart()->addItem(ID, quantity);
    this->showMessage("Tuyệt!", "Sản phẩm đã được thêm vào giỏ hàng", false);
}


void MainWindow::on_buyNowBtn_clicked()
{
    if (!this->checkBuyerRole()) {
        return;
    }

    QString ID = ui->productID->whatsThis();
    uint stock = ui->productStock->text().toUInt();

    bool check = false;
    uint quantity = ui->productBuyQuantity->text().toUInt(&check);

    if (!check || quantity == 0) {
        this->showMessage("Lỗi!", "Thông tin nhập vào không hợp lệ", true);
        return;
    }

    if (quantity > stock) {
        this->showMessage("Lỗi!", "Số lượng sản phẩm có sẵn không đủ", true);
        return;
    }

    this->renderCheckoutDialog(ID, quantity);
}

void MainWindow::on_accountBtn_clicked()
{
    this->renderAccountInfor();
    this->setInfoReadonly(true);
    this->resetPages();

    if (_role == SELLER) {
        ui->pages->setCurrentIndex(SELLER_PAGE);
    }
    else if (_role == STAFF) {
        ui->pages->setCurrentIndex(STAFF_PAGE);
    }
    else if (_role == BUYER) {
        ui->pages->setCurrentIndex(BUYER_PAGE);
    }
    else {
        ui->pages->setCurrentIndex(ADMIN_PAGE);
    }

    ui->searchInput->clear();
}

////////////////////////////////////////////////////////////////////////////////////
// Login page

void MainWindow::on_signinBtn_clicked()
{
    string username = ui->signinUsername->text().toStdString();
    if (_accountProvider->getAccount(username) != nullptr) {
        this->showMessage("Lỗi đăng ký!", "Tên đăng nhập đã tồn tại", true);
        return;
    }

    wstring name = ui->signinName->text().toStdWString();
    string phoneNumber = ui->signinPhoneNumber->text().toStdString();
    wstring address = ui->signinAddress->text().toStdWString();
    string password = ui->signInPassword->text().toStdString();
    if (name.size() == 0 || phoneNumber.size() == 0 || address.size() == 0 || password.size() == 0) {
        this->showMessage("Lỗi đăng ký!", "Vui lòng nhập đầy đủ thông tin", true);
        return;
    }

    Account* account = nullptr;

    if (ui->signinRole->currentIndex() == 0) {
        account = new Buyer(username, name, phoneNumber, address);
    } else {
        account = new Seller(username, name, phoneNumber, address);
    }

    _accountProvider->addAccount(account);
    _loginProvider->addAccount(username, password);

    this->showMessage("Chào mừng đến với Bily!", "Bạn đã đăng ký thành công", false);
    this->clearLoginInput();
}


void MainWindow::on_loginBtn_clicked()
{
    string username = ui->loginUsername->text().toStdString();
    string password = ui->loginPassword->text().toStdString();

    bool check = _loginProvider->login(username, password);
    if (!check) {
        this->showMessage("Lỗi đăng nhập!", "Tài khoản hoặc mật khẩu không đúng", true);
    }
    else {
        _account = _accountProvider->getAccount(username);
        _role = _account->role();

        // Set the avatar to accountBtn
        QString avt = "buyer.png";
        if (_role == SELLER) avt = "seller.png";
        else if (_role == STAFF) avt = "staff.png";
        else if (_role == ADMIN) avt = "admin.png";

        QString avtStyle = "border-image: url(:/images/social/" + avt + ") 0 0 0 0 stretch stretch;";
        QString avtSize = "min-width: 50px; max-width: 50px; min-height: 50px; max-height: 50px; border-radius: 25px;";
        ui->accountBtn->setStyleSheet(avtStyle + avtSize);

        // Name of account
        QStringList nameWords = QString::fromStdWString(_account->name()).split(" ");
        QString name = nameWords[nameWords.size() - 1];
        ui->accountName->setText(name);

        // Set the account (or username) for sub-dialogs
        string username = _account->username();
        _productDialog->setUsername(username);
        _voucherDialog->setUsername(username);
        _eventDialog->setUsername(username);
        _checkoutDialog->setUsername(username);

        this->loginFinished();
    }
}


void MainWindow::on_forgetPasswordBtn_clicked()
{
    ui->loginTab->setCurrentIndex(1);
    this->clearLoginInput();
}


void MainWindow::on_forgetBtnBack_clicked()
{
    ui->loginTab->setCurrentIndex(0);
}

void MainWindow::on_forgetPasswordConfirm_clicked()
{
    string username = ui->forgetPasswordUsername->text().toStdString();
    string phoneNumber = ui->forgetPasswordPhonenumber->text().toStdString();

    Account* account = _accountProvider->getAccount(username);
    if (!account || account->phoneNumber() != phoneNumber) {
        this->showMessage("Lỗi xác thực!", "Thông tin nhập vào không đúng", true);
    } else {
        _loginProvider->recoverPassword(username);

        int reply = this->showMessage("Lấy lại mật khẩu thành công!", "Mật khẩu mới của bạn là: 123456", false);
        if (reply == QMessageBox::Ok) {
            ui->loginTab->setCurrentIndex(0);
            this->clearLoginInput();
        }
    }
}

void MainWindow::on_backToShopBtn_clicked()
{
    ui->pages->setCurrentIndex(HOME_PAGE);
}

void MainWindow::on_productRemoveBtn_clicked()
{
    QString message = "Bạn có chắc chắn muốn xóa sản phẩm này không?";
    int reply = this->showOptionMessage("Xác nhận thao tác", message, "Có", "Không");

    if (reply == QMessageBox::StandardButton::No) {
        return;
    }

    this->showMessage("Thành công", "Bạn đã xóa sản phẩm có mã " + ui->productRemoveBtn->whatsThis(), false);
    _productProvider->removeProduct(ui->productRemoveBtn->whatsThis().toStdString());
    this->on_logo_clicked();
}

////////////////////////////////////////////////////////////////////////////////////
// Buyer's page

// Buyer's info

// Info's section button
void MainWindow::on_buyerInfoBtn_clicked()
{
    this->clearScrollData();

    this->changeAccountSection(_currentBuyerSection, ui->buyerInfoBtn);

    ui->buyerView->setCurrentIndex(BUYER_INFO);
    ui->buyerInfoTab->setCurrentIndex(0);

    this->renderAccountInfor();
    this->setInfoReadonly(true);
}


void MainWindow::on_buyerInfoTab_currentChanged(int index)
{
    switch (index) {
    case 0:
        this->renderAccountInfor();
        this->setInfoReadonly(true);
        break;
    case 1:
        ui->buyerOldPassword->clear();
        ui->buyerNewPassword->clear();
        ui->buyerNewPasswordConfirm->clear();
        break;
    case 2:
        ui->buyerDepositInput->clear();
        ui->buyerWithdrawInput->clear();
        break;
    default:
        break;
    }
}


void MainWindow::on_buyerEditInfo_clicked()
{
    this->setInfoReadonly(false);
}


void MainWindow::on_buyerSaveInfo_clicked()
{
    wstring name = ui->buyerName->text().toStdWString();
    string phoneNumber = ui->buyerPhoneNumber->text().toStdString();
    wstring address = ui->buyerAddress->text().toStdWString();

    _account->editInfo(name, phoneNumber, address);

    this->setInfoReadonly(true);
    this->renderAccountInfor();
}


void MainWindow::on_buyerPasswordBtn_clicked()
{
    string oldPassword = ui->buyerOldPassword->text().toStdString();
    string newPassword = ui->buyerNewPassword->text().toStdString();
    string confirmPassword = ui->buyerNewPasswordConfirm->text().toStdString();

    this->changePassword(oldPassword, newPassword, confirmPassword);
}

// -----------------------------------------------------------------------
// Buyer's wallet

void MainWindow::on_buyerDepositBtn_clicked()
{
    this->accountDeposit();
}

void MainWindow::on_buyerWithdrawBtn_clicked()
{
    this->accountWithdraw();
}

// -----------------------------------------------------------------------
// Buyer's cart section button

void MainWindow::on_buyerCartBtn_clicked()
{
    this->clearScrollData();

    this->changeAccountSection(_currentBuyerSection, ui->buyerCartBtn);
    ui->buyerView->setCurrentIndex(BUYER_CART);

    this->renderBuyerCart();
}

void MainWindow::on_buyerCheckoutBtn_clicked()
{
    this->renderCheckoutDialog();
}

// // --------------------------------------------------------------------

// Buyer's orders section button

void MainWindow::on_buyerOrdersBtn_clicked()
{
    this->clearScrollData();

    this->changeAccountSection(_currentBuyerSection, ui->buyerOrdersBtn);

    ui->buyerView->setCurrentIndex(BUYER_ORDERS);
    ui->buyerOrdersTab->setCurrentIndex(0);

    this->renderOrderList(true);
}


void MainWindow::on_buyerOrdersTab_currentChanged(int index)
{
    bool accepted = index == 0;
    this->renderOrderList(accepted);
}

// -----------------------------------------------------------------------
// Buyer's logout

void MainWindow::on_buyerLogoutBtn_clicked()
{
    this->clearScrollData();
    this->logout();
}


////////////////////////////////////////////////////////////////////////////////////
// Seller's page

// Seller's info

// Info's section Button
void MainWindow::on_sellerInfoBtn_clicked()
{
    this->clearScrollData();

    this->changeAccountSection(_currentSellerSection, ui->sellerInfoBtn);

    ui->sellerView->setCurrentIndex(SELLER_INFO);
    ui->sellerInfoTab->setCurrentIndex(0);

    this->renderAccountInfor();
    this->setInfoReadonly(true);
}


void MainWindow::on_sellerInfoTab_currentChanged(int index)
{
    switch (index) {
    case 0:
        this->renderAccountInfor();
        this->setInfoReadonly(true);
        break;
    case 1:
        ui->sellerOldPassword->clear();
        ui->sellerNewPassword->clear();
        ui->sellerNewPasswordConfirm->clear();
        break;
    case 2:
        ui->sellerDepositInput->clear();
        ui->sellerWithdrawInput->clear();
        break;
    default:
        break;
    }
}


void MainWindow::on_sellerEditInfo_clicked()
{
    this->setInfoReadonly(false);
}


void MainWindow::on_sellerSaveInfo_clicked()
{
    wstring name = ui->sellerName->text().toStdWString();
    string phoneNumber = ui->sellerPhoneNumber->text().toStdString();
    wstring address = ui->sellerAddress->text().toStdWString();

    _account->editInfo(name, phoneNumber, address);

    this->setInfoReadonly(true);
    this->renderAccountInfor();
}

// Password

void MainWindow::on_sellerPasswordBtn_clicked()
{
    string oldPassword = ui->sellerOldPassword->text().toStdString();
    string newPassword = ui->sellerNewPassword->text().toStdString();
    string confirmPassword = ui->sellerNewPasswordConfirm->text().toStdString();

    this->changePassword(oldPassword, newPassword, confirmPassword);
}


// -----------------------------------------------------------------------
// Seller's wallet

void MainWindow::on_sellerDepositBtn_clicked()
{
    this->accountDeposit();
}

void MainWindow::on_sellerWithdrawBtn_clicked()
{
    this->accountWithdraw();
}


// -----------------------------------------------------------------------
// Seller's shop

// Shop's section Button
void MainWindow::on_sellerShopBtn_clicked()
{
    this->clearScrollData();

    this->changeAccountSection(_currentSellerSection, ui->sellerShopBtn);

    ui->sellerProductsScroll->setWidgetResizable(true);
    ui->sellerProductsScroll->setWidget(this->renderProductList(ui->sellerProductsQuantity));

    ui->sellerView->setCurrentIndex(SELLER_SHOP);
}

void MainWindow::on_sellerAddProductBtn_clicked()
{
    ((FlexDialog*)_productDialog)->setType(false);
    _productDialog->exec();
}

// -----------------------------------------------------------------------
// Seller's orders

// Orders's section Button
void MainWindow::on_sellerOrdersBtn_clicked()
{
    this->clearScrollData();

    this->changeAccountSection(_currentSellerSection, ui->sellerOrdersBtn);

    ui->sellerView->setCurrentIndex(SELLER_ORDERS);
    ui->sellerOrdersTab->setCurrentIndex(0);
    this->renderOrderList(true);
}

void MainWindow::on_sellerOrdersTab_currentChanged(int index)
{
    bool accepted = index == 0;
    this->renderOrderList(accepted);
}

// -----------------------------------------------------------------------
// Voucher's statistics

void MainWindow::on_sellerVoucherBtn_clicked()
{
    this->clearScrollData();
    this->changeAccountSection(_currentSellerSection, ui->sellerVoucherBtn);
    ui->sellerView->setCurrentIndex(SELLER_VOUCHERS);
    this->renderSellerVouchers();
}

void MainWindow::on_sellerAddVoucherBtn_clicked()
{
    if (((Seller*)_account)->products().empty()) {
        this->showMessage("Lỗi", "Bạn chỉ tạo được Voucher khi đã có sản phẩm được đăng lên!", true);
        return;
    }

    ((FlexDialog*)_voucherDialog)->setType(false);
    _voucherDialog->loadInfor();
    _voucherDialog->exec();
}

// -----------------------------------------------------------------------
// Seller's statistics

// Statistics's section button
void MainWindow::on_sellerStatsBtn_clicked()
{
    this->changeAccountSection(_currentSellerSection, ui->sellerStatsBtn);
    ui->sellerView->setCurrentIndex(SELLER_STATS);
    this->renderStatistics();
}

// -----------------------------------------------------------------------
// Seller's logout button

void MainWindow::on_sellerLogoutBtn_clicked()
{
    this->clearScrollData();
    this->logout();
}

////////////////////////////////////////////////////////////////////////////////////
// Staff's page

// Staff's infor
void MainWindow::on_staffInfoBtn_clicked()
{
    this->clearScrollData();

    this->changeAccountSection(_currentStaffSection, ui->staffInfoBtn);

    ui->staffView->setCurrentIndex(STAFF_INFO);
    ui->staffInfoTab->setCurrentIndex(0);

    this->renderAccountInfor();
    this->setInfoReadonly(true);
}

void MainWindow::on_staffInfoTab_currentChanged(int index)
{
    switch (index) {
    case 0:
        this->renderAccountInfor();
        this->setInfoReadonly(true);
        break;
    case 1:
        ui->staffOldPassword->clear();
        ui->staffNewPassword->clear();
        ui->staffNewPasswordConfirm->clear();
        break;
    case 2:
        ui->staffDepositInput->clear();
        ui->staffWithdrawInput->clear();
        break;
    case 3: {
            Staff* staff = (Staff*)_account;
            ui->staffBaseSalary->setText(QString::fromStdWString(vndPrice(staff->baseSalary())));
            ui->staffBonus->setText(QString::fromStdWString(vndPrice(staff->bonus())));
            ui->staffTotalSalary->setText(QString::fromStdWString(vndPrice(staff->salary())));
            break;
        }
    default:
        break;
    }
}

void MainWindow::on_staffEditInfo_clicked()
{
    this->setInfoReadonly(false);
}

void MainWindow::on_staffSaveInfo_clicked()
{
    wstring name = ui->staffName->text().toStdWString();
    string phoneNumber = ui->staffPhoneNumber->text().toStdString();
    wstring address = ui->staffAddress->text().toStdWString();

    _account->editInfo(name, phoneNumber, address);

    this->setInfoReadonly(true);
    this->renderAccountInfor();
}

// Password

void MainWindow::on_staffPasswordBtn_clicked()
{
    string oldPassword = ui->staffOldPassword->text().toStdString();
    string newPassword = ui->staffNewPassword->text().toStdString();
    string confirmPassword = ui->staffNewPasswordConfirm->text().toStdString();

    this->changePassword(oldPassword, newPassword, confirmPassword);
}


// -----------------------------------------------------------------------
// Staff's wallet

void MainWindow::on_staffDepositBtn_clicked()
{
    this->accountDeposit();
}

void MainWindow::on_staffWithdrawBtn_clicked()
{
    this->accountWithdraw();
}


// -----------------------------------------------------------------------
// Seller's shop

void MainWindow::on_staffShopBtn_clicked()
{
    this->clearScrollData();

    this->changeAccountSection(_currentStaffSection, ui->staffShopBtn);

    ui->staffProductsScroll->setWidgetResizable(true);
    ui->staffProductsScroll->setWidget(this->renderProductList(ui->staffProductsQuantity));

    ui->staffView->setCurrentIndex(STAFF_SHOP);
}

void MainWindow::on_staffAddProductBtn_clicked()
{
    ((FlexDialog*)_productDialog)->setType(false);
    _productDialog->exec();
}


// -----------------------------------------------------------------------
// Seller's orders

void MainWindow::on_staffOrdersBtn_clicked()
{
    this->clearScrollData();

    this->changeAccountSection(_currentStaffSection, ui->staffOrdersBtn);

    ui->staffView->setCurrentIndex(STAFF_ORDERS);
    ui->staffOrdersTab->setCurrentIndex(0);
    this->renderOrderList(true);
}

void MainWindow::on_staffOrdersTab_currentChanged(int index)
{
    bool accepted = index == 0;
    this->renderOrderList(accepted);
}

// -----------------------------------------------------------------------
// Staff's event

void MainWindow::on_staffEventsBtn_clicked() {
    this->changeAccountSection(_currentStaffSection, ui->staffEventsBtn);
    ui->staffView->setCurrentIndex(STAFF_EVENTS);
    this->renderStaffEvents();
}


void MainWindow::on_staffAddEventBtn_clicked() {
    ((FlexDialog*)_eventDialog)->setType(false);
    _eventDialog->exec();
}

// -----------------------------------------------------------------------
// Staff's statistics

void MainWindow::on_staffStatsBtn_clicked()
{
    this->clearScrollData();

    this->changeAccountSection(_currentStaffSection, ui->staffStatsBtn);
    ui->staffView->setCurrentIndex(STAFF_STATS);
    this->renderStatistics();
}

// -----------------------------------------------------------------------
// Seller's logout

void MainWindow::on_staffLogoutBtn_clicked()
{
    this->clearScrollData();
    this->logout();
}

////////////////////////////////////////////////////////////////////////////////////
// Admin's page

// Admin's info
void MainWindow::on_adminInfoBtn_clicked()
{
    this->clearScrollData();

    this->changeAccountSection(_currentAdminSection, ui->adminInfoBtn);

    ui->adminView->setCurrentIndex(ADMIN_INFO);
    ui->adminInfoTab->setCurrentIndex(0);

    this->renderAccountInfor();
    this->setInfoReadonly(true);
}

void MainWindow::on_adminInfoTab_currentChanged(int index)
{
    switch (index) {
    case 0:
        this->renderAccountInfor();
        this->setInfoReadonly(true);
        break;
    case 1:
        ui->adminOldPassword->clear();
        ui->adminNewPassword->clear();
        ui->adminNewPasswordConfirm->clear();
        break;
    case 2:
        ui->adminDepositInput->clear();
        ui->adminWithdrawInput->clear();
        break;
    default:
        break;
    }
}


void MainWindow::on_adminEditInfo_clicked()
{
    this->setInfoReadonly(false);
}


void MainWindow::on_adminSaveInfo_clicked()
{
    wstring name = ui->adminName->text().toStdWString();
    string phoneNumber = ui->adminPhoneNumber->text().toStdString();
    wstring address = ui->adminAddress->text().toStdWString();

    _account->editInfo(name, phoneNumber, address);

    this->setInfoReadonly(true);
    this->renderAccountInfor();
}

// Password
void MainWindow::on_adminPasswordBtn_clicked()
{
    string oldPassword = ui->adminOldPassword->text().toStdString();
    string newPassword = ui->adminNewPassword->text().toStdString();
    string confirmPassword = ui->adminNewPasswordConfirm->text().toStdString();

    this->changePassword(oldPassword, newPassword, confirmPassword);
}


// -----------------------------------------------------------------------
// Admin's wallet

void MainWindow::on_adminDepositBtn_clicked()
{
    this->accountDeposit();
}

void MainWindow::on_adminWithdrawBtn_clicked()
{
    this->accountWithdraw();
}

// -----------------------------------------------------------------------
// Admin's staff

void MainWindow::on_adminStaffsBtn_clicked()
{
    this->clearScrollData();

    this->changeAccountSection(_currentAdminSection, ui->adminStaffsBtn);
    ui->adminView->setCurrentIndex(ADMIN_STAFFS);

    this->renderStaffList();
}

void MainWindow::on_adminStaffPayBtn_clicked()
{
    Admin* admin = (Admin*)_account;

    ullong total = admin->totalSalaries();

    QString message = "Tổng tiền lương tháng này của các nhân viên là " + QString::fromStdWString(vndPrice(total));
    message += "\nBạn có chắc chắn muốn thực hiện thao tác này?                        ";
    int reply = this->showOptionMessage("Xác nhận thao tác", message, "Có", "Không");

    if (reply == QMessageBox::StandardButton::No) {
        return;
    }

    bool check = ((Admin*)_account)->paySalaries();
    if (!check) {
        this->showMessage("Lỗi", "Số dư trong Ví Bily của bạn không đủ", true);
    } else {
        this->showMessage("Thành công", "Bạn đã trả lương thành công", false);
        this->renderStaffList();
    }
}

void MainWindow::on_adminStaffAddBtn_clicked()
{
    _addStaffDialog->loadInfor();
    _addStaffDialog->exec();
}

void MainWindow::on_adminSellersBtn_clicked()
{
    this->clearScrollData();

    this->changeAccountSection(_currentAdminSection, ui->adminSellersBtn);
    ui->adminView->setCurrentIndex(ADMIN_SELLERS);

    this->renderSellerList();
}

// -----------------------------------------------------------------------
// Admin's events

void MainWindow::on_adminEventsBtn_clicked()
{
    this->clearScrollData();

    this->changeAccountSection(_currentAdminSection, ui->adminEventsBtn);
    ui->adminView->setCurrentIndex(ADMIN_EVENTS);

    QFrame* container = this->renderVoucherList(ui->adminEventQuantity, true);
    ui->adminEventScroll->setWidgetResizable(true);
    ui->adminEventScroll->setWidget(container);
}

// -----------------------------------------------------------------------
// Admin's staffs

void MainWindow::on_adminStatsBtn_clicked()
{
    this->clearScrollData();

    this->changeAccountSection(_currentAdminSection, ui->adminStatsBtn);
    ui->adminView->setCurrentIndex(ADMIN_STATS);
    this->renderStatistics();
}

// -----------------------------------------------------------------------
// Admin's logout

void MainWindow::on_adminLogoutBtn_clicked()
{
    this->clearScrollData();
    this->logout();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Message notifications

int MainWindow::showMessage(QString title, QString message, bool isError)
{
    QMessageBox* messageBox = new QMessageBox;
    messageBox->setStyleSheet("QMessageBox { font-size: 16px; background-color: #f5f5fa; }");

    QPushButton* btn = messageBox->addButton(QMessageBox::StandardButton::Ok);
    btn->setCursor(Qt::PointingHandCursor);
    QString backgroundColor = isError ? "#f27d85" : "#62b7af";
    btn->setStyleSheet("QPushButton { border-radius: 15px; padding: 4px 16px; color: white; font-size: 16px; font-weight: bold; background-color: " + backgroundColor + ";}");

    messageBox->setText(message + "      ");
    messageBox->setWindowTitle(title);

    messageBox->setWindowIcon(QIcon(isError ? ":/images/icon/error.png" : ":/images/icon/done.png"));
    return messageBox->exec();
}

int MainWindow::showOptionMessage(QString title, QString message, QString option1, QString option2)
{
    QMessageBox* messageBox = new QMessageBox;
    messageBox->setStyleSheet("QMessageBox { font-size: 16px; background-color: #f5f5fa; }");

    QPushButton* okBtn = messageBox->addButton(QMessageBox::StandardButton::Ok);
    QPushButton* noBtn = messageBox->addButton(QMessageBox::StandardButton::No);

    okBtn->setCursor(Qt::PointingHandCursor);
    okBtn->setText(option1);
    noBtn->setCursor(Qt::PointingHandCursor);
    noBtn->setText(option2);

    QString backgroundColor = "#62b7af";
    okBtn->setStyleSheet("QPushButton { border-radius: 15px; padding: 4px 16px; color: white; font-size: 16px; font-weight: bold; background-color: " + backgroundColor + ";}");

    backgroundColor = "#f27d85";
    noBtn->setStyleSheet("QPushButton { border-radius: 15px; padding: 4px 16px; color: white; font-size: 16px; font-weight: bold; background-color: " + backgroundColor + ";}");


    messageBox->setText(message + "      ");
    messageBox->setWindowTitle(title);

    messageBox->setWindowIcon(QIcon(":/images/icon/note.png"));
    return messageBox->exec();
}

bool MainWindow::checkBuyerRole() {
    if (!_account) {
        this->showMessage("Lỗi!", "Vui lòng đăng nhập để sử dụng chức năng này!", true);
        return false;
    }

    if (_role != BUYER) {
        this->showMessage("Lỗi!", "Chỉ có Người Mua mới có thể sử dụng chức năng này!", true);
        return false;
    }

    return true;
}

void MainWindow::on_aboutUsBtn_clicked()
{
    this->clearScrollData();
    ui->pages->setCurrentIndex(ABOUT_US);
}

