# LrDesktopCleanerQt

铹哥桌面清理大师 Powered by Qt

---

- V0.1.0 Alpha 2020/06/25

# Description

自动清理桌面上放置的视频（可配置允许时长）
超过该时间后自动删除视频。

配置文件目录： `C:\Users\Admin\AppData\Local\LrDesktopCleanerQt\`

### interval.txt

默认：

```
3600
```

包含一个数字（单位：秒）设定文件保留时长

### storage.heap

不支持查看，以二进制方式持久化存储待删除的文件列表。

### log.txt

该版本中删除了日志记录。没有这个文件。
