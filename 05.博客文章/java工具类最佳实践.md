# java工具类最佳实践

## 前言
在使用java做开发中，我们总是会使用很多工具类，更多的时候第三方API使用很别扭，我们会自己去写工具类，类似下面这样的工具类：
![fail](https://cdn.jsdelivr.net/gh/pitifulnoble/picture@master/uPic/54699c42b2b6e5c6dda96b8448ccff28.png)

我们想想要如何使用上面的工具类，我要先``new``出来，然后再调用，代码有很多地方都要使用，这些对象更多的时候是可以服用的，管理这些对象太麻烦了(尤其是没有引入IOC/DI这类工具的时候)。
<br>
当前版本使用方法：
```java
HttpUtils utils = new HttpUtils();
String url = "https://helloworld.com";
String response = utils.doGet(url);
```

现在我们就来一步一步探讨如何让工具类的编写更加科学～

## static静态方案
既然工具类需要复用多次，实例化很麻烦，那用静态方法不就完美了吗，类似下面这样：
![fail](https://cdn.jsdelivr.net/gh/pitifulnoble/picture@master/uPic/c63cfedbedc74d25a7efa0d97820851d.png)

这个版本这样使用：
```java
String response = HttpUtils.doGet("https://helloworld.com");
```
天呐！简直不要太方便。我们解决了所有的问题。

但随着使用的次数增长，我们又有了新的问题，有些代码需要我们定制工具类的成员变量，比如给``client``设置不一样的超时时间。总有一天会出现这样的需求，但静态变量修改会全局可见(一个项目中，可能更多的时候使用默认配置就很合适，比如超时时间5秒，但某些逻辑必须限制超时时间1秒，如果我们更改静态成员变量，就会全局生效。)，这不能满足我们的需求，必须作出改变。

## 静态+实例组合
![fail](https://cdn.jsdelivr.net/gh/pitifulnoble/picture@master/uPic/9541750acdc56612223f6014edd4092c.png)
默认配置时我们使用instance实例类即可，如果有定制需求时client和headers都可以在外部set进来。