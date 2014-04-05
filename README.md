Lightup-简洁的系统级 C++ 库
=======

简介
-------

Lightup 是一个针对 Windows 平台的 C++ 库，它将 Windows 底层服务进行了现代化的封装，使你仅需使用少量代码就能调用各项强大的系统功能。

和传统的 MFC 之类的 C++ 库不同，Lightup 并不是以框架为导向的重量级库，因此没有复杂的继承链，文件依赖关系也非常简单。Lightup 的目标并不是要取代其他 C++ 库，而是作为你工具箱里的一个有益的补充，加速你的软件开发。

将繁琐变简单
--------

Lightup 外表简单，让人感觉性格直率，但却又是心思细密的。

例如，你只需两行代码，就可以将一个 UTF8 编码的文本文件读入为 wstring：

```cpp
std::wstring content;
FileSystem::ReadUtf8File(L"%temp%\\my_file.txt", content); // 注意文件路径中含有环境变量
```

为了完成这个简单的功能，Lightup 做了以下工作：

1. 将路径中的环境变量展开——使用 `ExpandEnvironmentStrings()`
2. 以只读权限打开文件——使用 `CreateFile()`
3. 获取文件尺寸——使用 `GetFileSizeEx()`
4. 创建文件原始内容缓冲——使用 `malloc()`
5. 读入文件内容——使用 `ReadFile()`
6. 创建转码后缓冲——使用 `malloc()`
7. 将 UTF8 转为 UTF16 LE ——使用 `MultiByteToWideChar()`
8. 将转码后缓冲转为 std::wstring

在上述步骤中的任何一步，都可能出现错误。Lightup 做了认真的错误处理，因此当错误发生时，不会异常终止，并且保证资源能够被正确的释放。
