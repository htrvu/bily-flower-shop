QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../Components/Account/Account.cpp \
    ../Components/Account/Admin.cpp \
    ../Components/Account/Buyer.cpp \
    ../Components/Account/Member.cpp \
    ../Components/Account/Seller.cpp \
    ../Components/Account/Staff.cpp \
    ../Components/Cart/Cart.cpp \
    ../Components/Date/Date.cpp \
    ../Components/Order/Order.cpp \
    ../Components/Product/Product.cpp \
    ../Components/Voucher/Voucher.cpp \
    ../Components/Wallet/Wallet.cpp \
    ../Providers/AccountProvider/AccountProvider.cpp \
    ../Providers/LoginProvider/LoginProvider.cpp \
    ../Providers/OrderProvider/OrderProvider.cpp \
    ../Providers/ProductProvider/ProductProvider.cpp \
    ../Providers/VoucherProvider/VoucherProvider.cpp \
    ../Utils/sha256/sha256.cpp \
    ../Utils/utils.cpp \
    ui_components/ListItem/SellerItem/selleritem.cpp \
    ui_components/ListItem/StaffItem/staffitem.cpp \
    ui_components/ListItem/StatisticItem/statisticitem.cpp \
    ui_components/ListItem/VoucherItem/voucheritem.cpp \
    ui_components/MyButton/mybutton.cpp \
    main.cpp \
    mainwindow.cpp \
    ui_components/ListItem/CartItem/cartitem.cpp \
    ui_components/ListItem/OrderItem/orderitem.cpp \
    ui_components/ListItem/ProductItem/productitem.cpp \
    ui_components/MyFrame/myframe.cpp \
    ui_components/ProductCard/productcard.cpp \
    ui_components/SubDialogs/CartEditDialog/carteditdialog.cpp \
    ui_components/SubDialogs/CheckoutDialog/checkoutdialog.cpp \
    ui_components/SubDialogs/EventDialog/eventdialog.cpp \
    ui_components/SubDialogs/OrderDialog/orderdetaildialog.cpp \
    ui_components/SubDialogs/OrderDialog/rateorderdialog.cpp \
    ui_components/SubDialogs/ProductDialog/productdialog.cpp \
    ui_components/SubDialogs/StaffDialog/addstaffdialog.cpp \
    ui_components/SubDialogs/StaffDialog/staffinfordialog.cpp \
    ui_components/SubDialogs/VoucherDialog/voucherdialog.cpp \
    ui_components/SubDialogs/flexdialog.cpp \
    ui_components/SubDialogs/mydialog.cpp

HEADERS += \
    ../CONSTANTS.h \
    ../Components/Account/Account.h \
    ../Components/Account/Admin.h \
    ../Components/Account/Buyer.h \
    ../Components/Account/Member.h \
    ../Components/Account/Seller.h \
    ../Components/Account/Staff.h \
    ../Components/Cart/Cart.h \
    ../Components/Date/Date.h \
    ../Components/Order/Order.h \
    ../Components/Product/Product.h \
    ../Components/Statistics/Statistics.h \
    ../Components/Voucher/Voucher.h \
    ../Components/Wallet/Wallet.h \
    ../LIBRARIES.h \
    ../Providers/AccountProvider/AccountProvider.h \
    ../Providers/LoginProvider/LoginProvider.h \
    ../Providers/OrderProvider/OrderProvider.h \
    ../Providers/ProductProvider/ProductProvider.h \
    ../Providers/VoucherProvider/VoucherProvider.h \
    ../Utils/json/json.hpp \
    ../Utils/sha256/sha256.h \
    ../Utils/utils.h \
    ui_components/ListItem/SellerItem/selleritem.h \
    ui_components/ListItem/StaffItem/staffitem.h \
    ui_components/ListItem/StatisticItem/statisticitem.h \
    ui_components/ListItem/VoucherItem/voucheritem.h \
    ui_components/MyButton/mybutton.h \
    mainwindow.h \
    ui_components/ListItem/CartItem/cartitem.h \
    ui_components/ListItem/OrderItem/orderitem.h \
    ui_components/ListItem/ProductItem/productitem.h \
    ui_components/MyFrame/myframe.h \
    ui_components/ProductCard/productcard.h \
    ui_components/SubDialogs/CartEditDialog/carteditdialog.h \
    ui_components/SubDialogs/CheckoutDialog/checkoutdialog.h \
    ui_components/SubDialogs/EventDialog/eventdialog.h \
    ui_components/SubDialogs/OrderDialog/orderdetaildialog.h \
    ui_components/SubDialogs/OrderDialog/rateorderdialog.h \
    ui_components/SubDialogs/ProductDialog/productdialog.h \
    ui_components/SubDialogs/StaffDialog/addstaffdialog.h \
    ui_components/SubDialogs/StaffDialog/staffinfordialog.h \
    ui_components/SubDialogs/VoucherDialog/voucherdialog.h \
    ui_components/SubDialogs/flexdialog.h \
    ui_components/SubDialogs/mydialog.h

FORMS += \
    mainwindow.ui \
    ui_components/SubDialogs/CartEditDialog/carteditdialog.ui \
    ui_components/SubDialogs/CheckoutDialog/checkoutdialog.ui \
    ui_components/SubDialogs/EventDialog/eventdialog.ui \
    ui_components/SubDialogs/OrderDialog/orderdetaildialog.ui \
    ui_components/SubDialogs/OrderDialog/rateorderdialog.ui \
    ui_components/SubDialogs/ProductDialog/productdialog.ui \
    ui_components/SubDialogs/StaffDialog/addstaffdialog.ui \
    ui_components/SubDialogs/StaffDialog/staffinfordialog.ui \
    ui_components/SubDialogs/VoucherDialog/voucherdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    ../Database/accounts.json \
    ../Database/orders.json \
    ../Database/passwords.json \
    ../Database/products.json \
    ../Utils/sha256/LICENSE

