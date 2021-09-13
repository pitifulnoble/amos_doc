
期中集群架构-第六章-ansible批量管理服务介绍
======================================================================

__01. 批量管理服务知识介绍__<br>
- a. ansible是一个基于Python开发的自动化运维工具
- b. ansible是一个基于ssh协议实现远程管理的工具
- c. ansible软件可以实现多种批量管理操作（批量系统配置、批量软件部署、批量文件拷贝、批量运行命令）
- saltstack puppet

__02. 批量管理服务特征介绍__<br>
- a ansible软件服务端（管理端）：不需要启动任何服务 默认服务端不需要任何的配置
- b ansible软件客户端（受控端）：没有客户端软件安装

__03. ansible软件安装部署__<br>
- a ansible软件自动化环境架构规划<br>
　　　管理主机1台：<br>
　　　10.0.0.61　m01<br>
　　　受控主机3台：<br>
　　　10.0.0.41　backup<br>
　　　10.0.0.31　nfs01<br>
　　　10.0.0.7　　web01<br>
　　　Linux系统 6.9<br>
- b ansible软件自动化部署条件<br>
　　建议基于ssh密钥方式建立远程连接<br>
    - a ssh密钥对创建（管理主机）<br>
　　　　　ssh-keygen -t dsa<br>
　　　　　影响免交互创建密钥对创建因素：<br>
　　　　　1）需要指定私钥存放路径<br>
　　　　　   -f /root/.ssh/id_dsa<br>
　　　　　2）需要进行私钥文件密码设定<br>
　　　　　   -N/-P<br>
　　　　　   -N ""　/　-P ""<br>

　　　　　　　　　　免交互创建密钥对方法<br>
　　　　　　　　　　``ssh-keygen -t dsa -f /root/.ssh/id_dsa -N ""``<br>

- c 分发公钥文件（管理主机进行分发）<br>
	    ``ssh-copy-id -i /root/.ssh/id_dsa.pub 172.16.1.31``<br>
		影响免交互批量分发密钥因素<br>
    - 1）需要有确认连接过程，需要输入yes/no<br>
		   ``-o StrictHostKeyChecking=no``<br>
		   ``sshpass -p'password' ssh-copy-id -i /root/.ssh/id_dsa.pub "-o StrictHostKeyChecking=no 172.16.1.31"``<br>

    - 2）需要解决密码问题
       ```
       sshpass -p'password' ssh-copy-id -i /root/.ssh/id_dsa.pub 172.16.1.31<br>
       Now try logging into the machine, with "ssh '172.16.1.31'", and check in:<br>

         .ssh/authorized_keys

       to make sure we haven't added extra keys that you weren't expecting.
       ```

		免交互批量分发公钥脚本：<br>
        ```
        #!/bin/bash
        rm /root/.ssh/id_dsa
        ssh-keygen -t dsa -f /root/.ssh/id_dsa -N ""

        for ip in 31 41 7
        do
        sshpass -p'password' ssh-copy-id -i /root/.ssh/id_dsa.pub "-o StrictHostKeyChecking=no 172.16.1.$ip"
        done
        ```

- d 检查是否可以进行基于密钥远程管理<br>
``ssh 172.16.1.31 uptime``<br>
    - 免交互批量检查测试脚本<br>
        ```
        #!/bin/bash
        rm /root/.ssh/id_dsa
        ssh-keygen -t dsa -f /root/.ssh/id_dsa -N ""

        for ip in 31 41 7
        do
        sshpass -p'password' ssh-copy-id -i /root/.ssh/id_dsa.pub "-o StrictHostKeyChecking=no 172.16.1.$ip"
        done
        ```

    - 基于ssh口令方式建立远程连接（也可以）<br>
         ```
        vim /etc/ansible/hosts
        [oldboy]
        172.16.1.7
        172.16.1.31 ansible_user=root ansible_password='password'
        172.16.1.41

        ansible 172.16.1.31 -m command -a "hostname" -k     --- 实现口令交互式远程管理
        SSH password:
        172.16.1.31 | SUCCESS | rc=0 >>
        nfs01
        ```

- e ansible软件下载安装<br>
　　　　ansible管理主机软件安装：<br>
　　　　``yum install -y ansible``<br>
　　　　ansible受控主机软件安装：（可选）<br>
　　　　``yum install -y libselinux-python``<br>

- f ansible软件受控主机添加配置
    - 在控制端添加
       ```
       cat /etc/ansible/hosts
       [oldboy]
       172.16.1.7
       172.16.1.31
       172.16.1.41
       ```

__04. ansible软件应用过程__<br>
- ansible软件模块<br>
  ```
  ansible-doc -l|wc -l
  1378
  ```

	ansible 管理主机信息或者主机组信息  -m 模块名称 -a 相关模块参数

	主机信息：远程主机IP地址  远程主机组名称  远程所有主机all
	-m 指定相应模块
	-a 利用模块中某些参数功能

    - 命令类型模块：
        - 第一个模块：command<br>
	        官方参考链接：http://docs.ansible.com/ansible/latest/modules/command_module.html<br>
	        参数：chdir---在执行莫个命令前，先切换目录<br>
	        ```
	        [root@m01 ansible]# ansible 172.16.1.31 -m command -a "chdir=/tmp/ pwd"
	        172.16.1.31 | SUCCESS | rc=0 >>
            /tmp
            ```
            ```
            [root@m01 ansible]# ansible 172.16.1.31 -m command -a "chdir=/etc/ pwd"
            172.16.1.31 | SUCCESS | rc=0 >>
            /etc
            ```
	        参数：creates---判断一个文件是否存在，如果已经存在了，后面的命令就不会执行<br>
	        ```
	        [root@m01 ansible]# ansible 172.16.1.41 -m command -a "creates=/etc/rsyncd.conf hostname"
            172.16.1.41 | SUCCESS | rc=0 >>
            skipped, since /etc/rsyncd.conf exists
            ```
            ```
            [root@m01 ansible]# ansible 172.16.1.41 -m command -a "creates=/etc/rsyncd.conf.bak hostname"
            172.16.1.41 | SUCCESS | rc=0 >>
            skipped, since /etc/rsyncd.conf.bak exists
            ```
            ```
            [root@m01 ansible]# ansible 172.16.1.41 -m command -a "creates=/etc/rsyncd.123456 hostname"
            172.16.1.41 | SUCCESS | rc=0 >>
            backup
            ```
	        参数：removes---判断一个文件是否存在，如果不存在，后面的命令就不会执行<br>
	        ```
            [root@m01 ansible]# ansible 172.16.1.41 -m command -a "removes=/etc/rsyncd.conf hostname"
            172.16.1.41 | SUCCESS | rc=0 >>
            backup
            ```
            ```
            [root@m01 ansible]# ansible 172.16.1.41 -m command -a "removes=/etc/rsyncd.1212213123 hostname"
            172.16.1.41 | SUCCESS | rc=0 >>
            skipped, since /etc/rsyncd.1212213123 does not exist
            ```
	        参数（必须要有的）：free_form---表示执行command模块时，必须要有linux合法命令信息<br>
	        ```
	        ansible 172.16.1.41 -m command -a "ls"
            172.16.1.41 | SUCCESS | rc=0 >>
            1
            anaconda-ks.cfg
            dead.letter
            heqing
            ```
        - 第二个模块：shell模块(万能模块)<br>
	        参数：chdir---在执行莫个命令前，先切换目录<br>
	        参数：creates---判断一个文件是否存在，如果已经存在了，后面的命令就不会执行<br>
	        参数：removes---判断一个文件是否存在，如果不存在，后面的命令就不会执行<br>
	        参数（必须要有的）：free_form---表示执行command模块时，必须要有linux合法命令信息<br>
            ```
            [root@m01 ansible]# ansible 172.16.1.41 -m shell -a "ls;pwd"
            172.16.1.41 | SUCCESS | rc=0 >>
            1
            anaconda-ks.cfg
            dead.letter
            /root
            ```
	        说明：shell模块可以满足command模块所有功能，并且可以支持识别特殊字符信息 < > | ;<br>

        - 第三个模块：script---专门运行脚本模块<br>
            参数：chdir---在执行莫个命令前，先切换目录<br>
	        参数：creates---判断一个文件是否存在，如果已经存在了，后面的命令就不会执行<br>
	        参数：removes---判断一个文件是否存在，如果不存在，后面的命令就不会执行<br>
	        参数（必须要有的）：free_form---表示执行command模块时，必须要有linux合法命令信息<br>

- 文件类型模块：
    - 第一个模块：copy----复制模块<br>
	    - 参数：backup---对数据信息进行备份<br>
            ```
            [root@m01 ansible]# ansible 172.16.1.41 -m copy -a "src=/tmp/file01.txt dest=/tmp/ backup=yes"
            172.16.1.41 | SUCCESS => {
                "backup_file": "/tmp/file01.txt.71887.2018-04-02@23:33:19~",
                "changed": true,
                "checksum": "029b054db136cc36d5605e3818305825ff4b8ffb",
                "dest": "/tmp/file01.txt",
                "gid": 0,
                "group": "root",
                "md5sum": "434660b5ad7deeba8815349f71409405",
                "mode": "0644",
                "owner": "root",
                "size": 6,
                "src": "/root/.ansible/tmp/ansible-tmp-1522683197.05-52744169892601/source",
                "state": "file",
                "uid": 0
            }
            ```

        - 参数：src---定义要推送数据信息<br>
        - 参数：dest---定义将数据推送到远程主机什么目录中<br>
            ```
	        [root@m01 ansible]# touch /tmp/file01.txt
            [root@m01 ansible]# ansible 172.16.1.41 -m copy -a "src=/tmp/file01.txt dest=/tmp/"
            172.16.1.41 | SUCCESS => {
                "changed": true,
                "checksum": "da39a3ee5e6b4b0d3255bfef95601890afd80709",
                "dest": "/tmp/file01.txt",
                "gid": 0,
                "group": "root",
                "md5sum": "d41d8cd98f00b204e9800998ecf8427e",
                "mode": "0644",
                "owner": "root",
                "size": 0,
                "src": "/root/.ansible/tmp/ansible-tmp-1522682948.27-60532389065095/source",
                "state": "file",
                "uid": 0
            }
            ```
            ```
            [root@m01 ansible]# ansible 172.16.1.41 -m shell -a "ls -l /tmp/"
            172.16.1.41 | SUCCESS | rc=0 >>
            total 24
            -rw-r--r-- 1 root root    0 Apr  2 23:29 file01.txt
            ```
	    - 参数：owner---设置复制后的文件属主权限
	    - 参数：group---设置复制后的文件属组权限
	    - 参数：mode---设置复制后的文件权限（600 755）

	- 第二个模块：file----文件属性修改/目录创建/文件创建
	    - 参数：owner---设置复制后的文件属主权限
	    - 参数：group---设置复制后的文件属组权限
	    - 参数：mode---设置复制后的文件权限（600 755）
	        ```
	        ansible 172.16.1.41 -m file -a "dest=/tmp/file01.txt owner=oldboy group=oldboy mode=600"
            172.16.1.41 | SUCCESS => {
                "changed": true,
                "gid": 500,
                "group": "oldboy",
                "mode": "0600",
                "owner": "oldboy",
                "path": "/tmp/file01.txt",
                "size": 6,
                "state": "file",
                "uid": 500
            }
            ```
	    - 参数：state---用于指定创建目录或文件<br>
	        创建文件<br>
	        ```
	        ansible 172.16.1.41 -m file -a "dest=/tmp/file01.txt state=touch"
            172.16.1.41 | SUCCESS => {
                "changed": true,
                "dest": "/tmp/file01.txt",
                "gid": 0,
                "group": "root",
                "mode": "0644",
                "owner": "root",
                "size": 0,
                "state": "file",
                "uid": 0
            }
            ```
	        创建目录：<br>
	        ```
	        ansible 172.16.1.41 -m file -a "dest=/tmp/dir01 state=directory"
            172.16.1.41 | SUCCESS => {
                "changed": true,
                "gid": 0,
                "group": "root",
                "mode": "0755",
                "owner": "root",
                "path": "/tmp/dir01",
                "size": 4096,
                "state": "directory",
                "uid": 0
            }
            ```

- 包管理模块类型
	- 模块：yum---安装软件包模块
	    - name：执行要安装软件的名称，以及软件的版本
	    - state：installed安装  absent(卸载)<br>
	        ``ansible 172.16.1.41 -m yum -a "name=iftop state=installed"``<br>
	        ``ansible 172.16.1.41 -m yum -a "name=iftop state=absent"``<br>

	    - list：指定软件名称，查看软件是否可以安装，以及是否已经安装过了
            ``ansible 172.16.1.41 -m yum -a "list=iftop"``<br>

- 系统模块类型
	- 模块：service---管理服务状态模块
	    - name: 指定要管理的服务名称（管理的服务一定在chkconfig中可以看到）
	    - state：stopped started restarted reloaded
	    - enabled：yes表示服务开机自启动 no表示服务开机不要自动启动

	        ``ansible 172.16.1.41 -m service -a "name=crond state=started enabled=yes"``<br>

	- cron---定时任务模块
	    ``* * * * *  /bin/sh /server/scripts/test.sh &>/dev/null``<br>

        ```
	    minute=0-59 * */n , -   hour  day  month weekday  job='/bin/sh /server/scripts/test.sh &>/dev/null'

	    添加定时任务
	      ansible 172.16.1.41 -m cron -a "minute=0 hour=0 job='/bin/sh /server/scripts/test.sh &>/dev/null'"
        ansible 172.16.1.41 -m cron -a "name=oldboy02 minute=0 hour=0 job='/bin/sh /server/scripts/test.sh &>/dev/null'"

	    删除定时任务
	    ansible 172.16.1.41 -m cron -a "name=oldboy02 minute=0 hour=0 job='/bin/sh /server/scripts/test.sh &>/dev/null' state=absent"
	    ansible 172.16.1.41 -m cron -a "name=oldboy01 state=absent"

	    注释定时任务
	    ansible 172.16.1.41 -m cron -a "name=oldboy01 minute=0 hour=0 job='/bin/sh /server/scripts/test.sh &>/dev/null' disabled=yes"
	    ansible 172.16.1.41 -m cron -a "name=oldboy01 job='/bin/sh /server/scripts/test.sh &>/dev/null' disabled=no"
        ```
	总结ansible颜色信息：<br>
	绿色：查看远程主机信息，不会对远程主机系统做任何修改<br>
	红色：执行操作出现异常错误<br>
	黄色：对远程主机系统进行修改操作<br>
	粉色：警告或者忠告信息<br>


- ansible软件剧本<br>
	编写剧本规范：<br>
	[地址](http://docs.ansible.com/ansible/latest/reference_appendices/YAMLSyntax.html)<br>
	遵循pyyaml<br>
	①. - 用法说明，表示列表显示的内容<br>
	    水果信息：<br>
		  - 苹果<br>
		  - 香蕉<br>
		  - 西瓜<br>
    ②. : 用法说明：<br>
	    姓名: 张三<br>
		性别: 男<br>
		人员信息:<br>
		- 运维人员: sa<br>
		- 开发人员: dev<br>
		- 存储人员: dba<br>
	③. 空格 用法说明：<br>
	    对内容进行分级时，需要有两个空格表示分级<br>
		软件安装步骤:<br>
		  - 服务端安装步骤:<br>
		    第一个里程碑: 检查软件是否安装<br>
			第二个里程碑: 编写配置文件内容<br>
		  - 客户端安装步骤:<br>
	    补充：必须使用空格分隔ansible剧本级别，一定不要使用tab键进行分割<br>

    执行脚本方法：<br>
    ``ansible-playbook /etc/ansible/ansible-playbook/test.yaml``<br>
    ``ansible-playbook -C /etc/ansible/ansible-playbook/test.yaml``<br>

    作业：<br>
    01. nfs服务一键化部署<br>
    02. inotify/sersync软件一键化部署<br>
