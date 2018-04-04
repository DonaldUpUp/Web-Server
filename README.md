---
title: web服务器
tags: web
grammar_cjkRuby: true
---


# web服务器

这是本人在空余时间，使用C++完成的一个web服务器，可以实现http的GET方法访问，支持静态页面和动态页面。

请求处理上分别使用了四种方法：

 1. 单线程模式
 2. 单生产者单消费者模式
 3. 单生产者多消费者模式
 4. 多路IO模式
 
       使用apache的ab.exe工具，在1核CPU,1GB内存的虚拟机上进行测试，并对四种方法进行了比较：
	   
![四种方法的效率比较](https://github.com/771884434/Web-Server/blob/master/pic/1.png)
 
      以及柱状图，方便查看：

![四种方法每秒可以处理的事务](https://github.com/771884434/Web-Server/blob/master/pic/2.png)

![四种方法平均事务相应时间](https://github.com/771884434/Web-Server/blob/master/pic/3.png)


		可以看出单线程和单生产者单消费者模式在并发数量较少时，效率比较高。单生产者比多路IO在大部分情况下效率要高，但是多路IO再处理并发数量较多时，效率要比单生产者多消费者高一些。
		
		这个服务器还有很多不足之处，如：仅支持HTTP的GET方法，不支持HTTPS和POST方法等。返回值只有两种等。
		
       在做的过程中参考了很多大佬分享自己写的代码，这个项目中也参考了很多。感谢各位大佬的无私奉献！


 



 

 


