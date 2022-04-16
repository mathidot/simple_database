# simple_database

一个简单的数据库系统，支持查找，删除，插入等基本功能。
该数据库有以下简单的特性：

* 虚拟机
* 交互界面
* B-tree
* 分页存储

##  API使用简介

* .exit
* .btree
* insert 
* select
* delete  
* find(未完成)

### .exit:

    用法： .exit
    退出当前数据库程序，将缓存中的数据全部存储到硬盘上去。

### .btree:

```
用法： .btree
可视化数据库中的树状存储结构。
```

### insert:

    用法： insert <key> <user_name> <user_email>
    将key,user_name,user_email插入到数据库中。

### select:

    用法： select
    显示数据库中的所有记录

### delete:

    用法： delete <key>
    删除指定关键字(key)的记录。

## 代码结构

```
db 
    --build
    --src
    	--buffer.c
    	--pager.c
    	--table.c
    	--types.c
    	--cursor.c
    	--node.c
   		--statement.c
    	--types.h
    	--defs.h
    --main.c
    --test.rb
    --makefile
    
```

