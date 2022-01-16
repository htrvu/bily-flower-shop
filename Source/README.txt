Ứng dụng BILY FLOWER SHOP
Đồ án cuối kì - Phương pháp lập trình hướng đối tượng
Nhóm 6 - Lớp 20CTT1TN1
Học kì 1 - Năm học 2021-2022
Trường Đại học Khoa học Tự nhiên - ĐHQG TP.HCM

////////////////////////////////////////////////////////////////////////////////////////

1. Để build được mã nguồn này một cách dễ dàng nhất, hãy sử dụng IDE Qt Creator
	B1. Thay đổi các đường dẫn đến thư mục Database trong file CONSTANTS.h như sau:
		Sửa các đường dẫn "Database/...." thành "../Database/..."
	B2. Tạo thư mục Release ở cùng cấp với các thư mục như GUI, Providers,...
	B3. Vào thư mục GUI, sau đó mở project thông qua file "BILY.pro"
	B4. Thiết lập thư mục chứa kết quả của quá trình Build là thư mục Release ở B1
	B5. Build mã nguồn bằng tổ hợp phím Ctrl + B. Sau đó nhấn tổ hợp phím Ctrl + R để khởi động ứng dụng

2. Nếu có nhu cầu deploy ứng dụng, tùy vào đường dẫn đến thư mục Database trong file CONSTANTS.h để có thể đặt vị trí thư mục đó một cách phù hợp.
   Ví dụ: Nếu muốn đặt thư mục Database ở cùng cấp với file thực thi (BILY.exe), ta thực hiện các bước sau:
	B1. Đảm bảo các đường dẫn đến thư mục Database trong file CONSTANTS.h là "Database/..."
	B2. Build mã nguồn bằng IDE Qt Creator
	B3. Sử dụng windeployqt để deploy ứng dụng
	B4. Copy thư mục Database vào cùng cấp với file thực thi, sau đó khởi động ứng dụng
