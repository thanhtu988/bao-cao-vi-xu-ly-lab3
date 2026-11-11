# Lab 3 – Buttons/Switches: đèn giao thông ngã tư (Vi xử lý – Vi điều khiển)

**Sinh viên:** Đào Ngọc Thanh Tú – 2413841
**Giảng viên:** Lê Trọng Nhân
**Nội dung:** đèn giao thông ngã tư có 3 nút nhấn, 4 mode. Cả 10 exercise nằm trong một project STM32CubeIDE (theo Ex10), mô phỏng trên Proteus.

Báo cáo: [`BaoCao/Bao-cao-vi-xu-ly-lab3.pdf`](BaoCao/Bao-cao-vi-xu-ly-lab3.pdf)

## Cách build và mô phỏng

1. STM32CubeIDE → `File > Open Projects from File System…` → chọn thư mục `Lab3` → `Ctrl+B`.
2. Proteus (mở bằng *Run as administrator*) → mở `Proteus/lab3.pdsprj`. `Program File` của STM32 đã trỏ tới `..\Lab3\Debug\Lab3.hex` → Play.

## Cấu trúc thư mục

```
lab3/
├── Lab3/          project STM32CubeIDE (Debug/Lab3.hex để nạp Proteus)
│   └── Core/Src   input_reading.c, input_processing.c, led_display.c, timer.c, main.c
├── Proteus/       lab3.pdsprj
├── BaoCao/        báo cáo PDF + file LaTeX + ảnh (img/)
└── README.md
```

## Sơ đồ chân

| Chân | Nhãn | Nối |
|---|---|---|
| PA1 … PA6 | N_*, S_* (RED/AMBER/GREEN) | 6 đèn tuyến 1 (Bắc, Nam) |
| PA7 … PA12 | E_*, W_* (RED/AMBER/GREEN) | 6 đèn tuyến 2 (Đông, Tây) |
| PB0 … PB6 | SEG0 … SEG6 | đoạn a … g, chung cho 4 LED 7 đoạn (chung anode) |
| PB8, PB9 | EN0, EN1 | 2 LED 7 đoạn tuyến 1 (qua PNP) |
| PB10, PB11 | EN2, EN3 | 2 LED 7 đoạn tuyến 2 (qua PNP) |
| PB12, PB13, PB14 | BUTTON_1, 2, 3 | nút nhấn, điện trở 10K kéo lên 3.3V |

## Cách dùng

- **Mode 1** (khởi động): đèn chạy bình thường, mặc định ĐỎ 5 s, XANH 3 s, VÀNG 2 s. Mỗi tuyến hiện số giây còn lại.
- **Nút 1**: chuyển mode 1 → 2 → 3 → 4 → 1.
- **Mode 2 / 3 / 4** (chỉnh ĐỎ / VÀNG / XANH): 4 đèn cùng màu nhấp nháy 2 Hz. LED tuyến 1 hiện giá trị, LED tuyến 2 hiện số mode.
- **Nút 2**: tăng giá trị (1 – 99, quá 99 về 1). **Nút 3**: lưu giá trị.
- Từ mode 4 nhấn nút 1 thì về mode 1 và chạy lại với thời gian mới.
