# BBK 9588 / 9688 USB Microphone

[![Host CI](https://github.com/HelloClyde/bbk-9588-usb-microphone/actions/workflows/host-ci.yml/badge.svg)](https://github.com/HelloClyde/bbk-9588-usb-microphone/actions/workflows/host-ci.yml)
[![License](https://img.shields.io/badge/license-Apache--2.0-blue.svg)](LICENSE)

这是一个面向 BBK 9588/C200 与 9688/C100 的实验性 USB PCM 传输项目。
设备端 BDA 识别精确固件 profile，接管 JZ4720/JZ4730/JZ4740 UDC，把 SDK
录音 PCM 通过 CDC ACM Bulk IN 发给 PC；Windows 主机端使用系统自带的
`usbccgp`/`usbser`，保存 WAV、实时波形、PNG 和诊断日志。

项目不包含固件、NAND 镜像、原机应用或其他专有数据。

> [!WARNING]
> 当前版本不能在退出后热恢复 USB Mass Storage。退出 BDA 前必须先拔 USB，
> 随后重启设备；重启前不要重新连接 PC。

## 当前状态

已由真机验证：

- Windows 将 `VID_A4A5&PID_0556` 识别为 composite CDC device。
- `usbccgp` parent、`usbser` child 和 COM port 均正常启动。
- 16 kHz、16-bit、mono PCM 可通过 EP2 Bulk IN 连续传输。
- 15 秒采集得到 15,000 个完整 frame，序号缺口、校验错误和设备 underrun
  均为 0。
- 设备端同次运行完成 15,104 个有效 EP2 frame，IRQ 和事件队列无异常。

尚未实现或验证：

- 退出 BDA 后恢复系统 USB Mass Storage。
- 无需重启的 UDC 热切换。
- Linux/macOS 主机工具。
- 新增的 9588/JZ4720、9588/JZ4740、9688/JZ4730 和 9688/JZ4740
  profile 尚未完成各自的一次综合真机验收。

固件支持状态：

| 设备 / SoC | BDA | 状态 |
| --- | --- | --- |
| 9588/C200 JZ4730 | `9588UsbMic.bda` | 已验证的后端与相同内核 payload |
| 9688/C100 JZ4730 | `9588UsbMic.bda` | 静态候选，待真机验收 |
| 9588/C200 JZ4720 | `9588UsbMic.bda` | 静态候选，待真机验收 |
| 9588/C200 JZ4740 | `9588UsbMic.bda` | 静态候选，待真机验收 |
| 9688/C100 JZ4740 | `9588UsbMic.bda` | 静态候选，待真机验收 |

精确恢复镜像 SHA-256、ABI 地址和最小测试矩阵见
[`docs/firmware-compatibility.md`](docs/firmware-compatibility.md)。

当前退出流程必须是：先拔 USB，再退出 BDA，最后重启设备。不要在退出后直接
重新连接 PC；当前版本明确保留对应 UDC IRQ masked，并标记
`restart_required=1`。

## 目录

```text
device/
  build.ps1                  BDA 构建和校验入口
  assets/                    菜单图标 PNG 和可复现生成脚本
  firmware_abi.h             逆向得到的当前固件 ABI 定义
  include/                   多固件 ABI 适配层与界面代码
  src/main.c                 型号/芯片检测与统一后端分发入口
  src/usb_cdc_pcm_core.c     已验证 JZ4730 设备端实现
  src/usb_cdc_pcm_musb.c     indexed-MUSB 设备端实现
host/
  build.ps1                  Windows 主机工具构建入口
  capture.ps1                自动发现 COM、采集并保存诊断信息
  capture.cmd                双击运行入口
  bridge/                    CDC 到 VB-CABLE 的常驻托盘桥接程序
  src/                       .NET 6 Windows 主机源码
installer/
  build.ps1                  构建自包含的一键安装程序
  9588-usb-mic.iss           Inno Setup 安装定义
docs/
  protocol.md                64-byte PCM frame 和 USB descriptor
  hardware.md                JZ4730 UDC/固件假设与生命周期
  firmware-compatibility.md  精确固件哈希、profile 和验收状态
  verified-baseline.md       真机验证边界和可追溯哈希
sdk/                         固定提交的 bbk9588-bda-sdk submodule
scripts/test.ps1             离线构建和静态回归检查
```

`device/src/usb_cdc_pcm_core.c` 目前保留了探针阶段的历史条件分支。它是可构建、
可追溯的迁移起点，不代表最终模块划分。新的 CDC PCM 开发以本目录为准，后续
应逐步拆分 firmware ABI、UDC、capture、transport 和 UI。

## 依赖

设备端：

- Windows PowerShell 5.1 或 PowerShell 7。
- Python 3.10 或更高版本。
- 仓库自带的 `bbk9588-bda-sdk` Git submodule。
- `mipsel-none-elf-gcc` 和 `mipsel-none-elf-objcopy`。

主机端：

- Windows 10/11。
- .NET 6 SDK。
- Windows inbox `usbccgp` 和 `usbser` 驱动。

虚拟麦克风安装包构建还需要 .NET 10 SDK 和 Inno Setup 6.7 或更高版本。
目标 PC 不需要预装 .NET；安装包中的桥接程序为自包含应用。

检出源码时初始化 SDK submodule：

```powershell
git clone --recurse-submodules `
  https://github.com/HelloClyde/bbk-9588-usb-microphone.git
```

已有工作区执行：

```powershell
git submodule update --init --recursive
```

构建默认使用 `sdk` 中固定的 SDK 提交。`BBK9588_SDK_ROOT` 仍可用于 SDK
开发调试，`JZ4730_TOOLCHAIN_PREFIX` 可覆盖工具链路径。

## 构建设备端

```powershell
.\device\build.ps1
```

默认输出：

```text
out/9588UsbMic.bda
```

`9588UsbMic.bda` 先通过 SDK 检测机型与芯片，再核对精确固件代码签名，
随后自动选择 JZ4730 PCH-style 或 JZ4720/JZ4740 MUSB 后端；任一结果不匹配
都会在任何 UDC MMIO 前退出。

可覆盖 SDK、工具链和输出位置：

```powershell
.\device\build.ps1 `
  -SdkRoot C:\src\bbk9588-bda-sdk `
  -ToolchainPrefix C:\toolchains\bin\mipsel-none-elf- `
  -IconPath C:\temp\menu-icon.png `
  -OutputPath C:\temp\9588UsbMic.bda
```

仓库已包含默认图标 PNG。修改图标生成脚本后，可使用 Pillow 重新生成：

```powershell
python .\device\assets\generate_icon.py
```

## 构建与运行主机端

只构建：

```powershell
.\host\build.ps1
```

真机采集：

```powershell
.\host\capture.ps1 -WaitSeconds 120 -CaptureSeconds 15
```

也可以双击 `host\capture.cmd`。脚本会自动查找
`VID_A4A5&PID_0556` 对应的正常 COM port，并在 `out/` 生成：

```text
capture.wav
waveform.png
capture.log
pnp.log
setupapi.log
```

## Windows 虚拟麦克风

`host/bridge` 会在后台自动发现 `VID_A4A5&PID_0556` 的 COM 端口，将
16 kHz、mono、signed 16-bit PCM 重采样后写入 VB-CABLE 的 `CABLE Input`。
Windows 应用中选择 `CABLE Output (VB-Audio Virtual Cable)` 作为麦克风。

构建一键安装程序：

```powershell
.\installer\build.ps1
```

输出为 `out\installer\9588UsbMicSetup.exe`。安装器自提权、从 VB-Audio
官方下载并校验原始驱动包、安装桥接程序并注册登录自启。VB-CABLE 要求重启；
Windows 也可能显示自己的驱动确认框。首次安装驱动前，安装器会保存 Windows
当前的三个默认播放端点，安装后恢复它们，并将 `CABLE Output` 设置为三个默认
录音端点。因此日常播放仍走原扬声器，应用默认使用 9588 虚拟麦克风。当前外层
安装器未做 Authenticode 签名，正式公开发布前必须签名。

VB-CABLE 是 Donationware，不属于本项目的 Apache-2.0 源码。开发版安装器在
安装时从官方站点下载；离线捆绑或商业分发前应取得 VB-Audio 的书面许可或对应
批量授权。详见 `installer/vb-cable-notice.txt`。

## 真机运行顺序

1. 重启设备，保持 USB 未连接。
2. 运行 `9588UsbMic.bda`。程序会自动选择后端，直接开始录音并显示实时波形。
3. 波形界面的红色指示表示设备正在采集，连接 USB 后绿色 `PC LINK`
   指示表示 PC 正在读取音频。
4. PC 桥接程序会自动连接；需要保存独立 WAV 时可运行 `host\capture.cmd`。
5. 点击设备上的 `STOP & EXIT`，或按 Esc，停止录音并断开 CDC。
6. 看到重启提示后重启设备；重启前不要重新连接 USB。
7. 系统 USB/Mass Storage 只能在重启后恢复。

设备端每次运行覆盖写入一份精简摘要日志：

```text
A:\应用\数据\9588usbmic.log
```

日志只保留采集 profile、启动结果、退出原因、采集错误数、USB/PCM 计数和
`reboot_required`，不再写寄存器快照与 USB setup trace。

## 离线验证

```powershell
.\scripts\test.ps1
```

该命令构建并校验统一 BDA、构建主机工具，并检查固件 profile、MUSB
Full-Speed 时序、CDC frame 以及主机端未重新引入 HID 路径。它不连接真机。

## 发布打包

完成设备端和安装器构建后运行：

```powershell
.\scripts\package-release.ps1 -Version 0.1.4-rc.1
```

脚本会在对应的 `out\release\v<version>\` 目录汇总 BDA、Windows 安装器和
`SHA256SUMS.txt`。GitHub Release 中的二进制不提交进源码历史。

推送符合 `v<major>.<minor>.<patch>` 或带预发布后缀的 tag 后，Tagged Release
工作流会在干净的 Windows runner 上重新构建所有资产。例如：

```powershell
git tag -a v0.1.4-rc.1 -m "v0.1.4-rc.1"
git push origin v0.1.4-rc.1
```

CI 会固定并校验 BDA packer、MIPS 工具链和 Inno Setup，随后自动创建 GitHub
Release，上传 `9588UsbMic.bda`、`9588UsbMicSetup.exe` 和
`SHA256SUMS.txt`。带连字符后缀的版本会发布为 prerelease。

## 版本与兼容

当前 wire format 固定为 64 bytes，采样格式固定为 16 kHz、16-bit、mono。
修改 descriptor、endpoint、frame layout、采样率或退出时序时，必须同步更新
`docs/protocol.md`、主机解析器和 CHANGELOG。

真机成本高，提交顺序应是：

1. 完成离线构建、静态检查和主机 parser 测试。
2. 把多个相关变更合并成一个有明确判定条件的候选版本。
3. 只运行一次综合真机验收，并同时保存设备端和主机端日志。

## License

项目源代码使用 Apache License 2.0。具体边界与来源说明见 `NOTICE` 和
`docs/provenance.md`。商标、固件和第三方材料不包含在该许可中。
