# ATmega16-EFI
- Hệ thống mô phỏng lại hệ thống phun xăng điện tử (EFI) trong xe ô tô:
![image](https://github.com/user-attachments/assets/1cbf81b9-4f83-4d8f-b29e-02cdb08d9d43)
- Tốc độ quay của động cơ ô tô sẽ được đọc bởi encoder và gửi tín hiệu đến ATmega16 thông qua ngắt ngoài. Từ đó, ATmega16 sẽ tiến hành xử lý và điều khiển tốc độ quay của máy bơm xăng thông qua điều chế xung (PWM) sao cho cung cấp đủ lượng xăng cần thiết cho động cơ hoạt động.
- Mô phỏng hệ thống trong Proteus:
![image](https://github.com/user-attachments/assets/344c4acf-06d6-46e6-8dc5-b07ee812164d)
- Trạng thái của động cơ: bật/tắt, tăng tốc, giảm tốc sẽ được điều khiển qua nút bấm.
- Thông tin về tốc độ quay động cơ, máy bơm, áp suất ống dẫn xăng sẽ được hiển thị qua màn hình LCD.
- Kết quả mô phỏng:
  ![image](https://github.com/user-attachments/assets/0997c25e-8768-4a91-bf00-d17a9d6e9647)
  ![image](https://github.com/user-attachments/assets/40d04560-f506-4c1b-af9f-78c0fc87473b)
  ![image](https://github.com/user-attachments/assets/229cdd14-46bf-4eea-b481-16e9bf561104)
  ![image](https://github.com/user-attachments/assets/a82f8ee5-10ed-40c1-866f-b557ddae8a85)
* Chú thích: đường màu vàng: PWM động cơ; đường màu xanh: PWM máy bơm
- Trong trường hợp mô phỏng ở trên, tốc độ ban đầu của động cơ ở gần mức tối đa, ta tiến hành phân tích:
+ Do động cơ ở tốc độ cao mà bơm xăng hoạt động ở tốc độ thấp, nên xăng tiêu thụ nhiều hơn xăng bơm vào ==> áp suất của đường ống lúc đầu giảm xuống còn 3.8kg/cm2
+ Sau đó hệ thống liên tục thực hiện căn chỉnh tốc độ máy bơm dựa theo hàm đã lập trình, ta có thể thấy Duty Cycle và tốc độ của máy bơm dần tăng lên 
+ Đến khi đạt được tốc độ cần thiết, tốc độ của máy bơm sẽ duy trì ổn định và áp suất của đường ống sẽ là mức lý tưởng 4kg/cm2
==> Trường hợp tốc độ máy bơm quá lớn so với tốc độ động cơ sẽ tương tự nhưng ngược lại






