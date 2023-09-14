# python的相关学习
## anconda和python
这里需要注意的是，在安装torch库的时候，一般是需要将，torch,cuda,cudnn这几个库进行版本指定的联合安装的，单独装很容易出错。
其次conda内的虚拟环境在寻找三方包时，需要先在python虚拟环境自己安装的包去寻找，然后再去conda安装的三方库去寻找。
### pyinstaller和conda虚拟环境
在pyinstaller打包的时候，可以分为一次打包为一个可执行文件或者，打包三方库，可执行文件在一个目录下。
最终生成的可执行文件是在dist目录下。三方库也在该目录下。
如果存在python的虚拟环境中打包时，涉及到了conda的三方库。如果打包不成功，即可执行文件缺少一些三方库。可以尝试的方向：
- 将本地python的pyinstaller转移到conda中，然后利用conda中的pyinstaller来进行打包。
- 利用conda base环境，在conda base环境中来使用conda list中的pyinstaller来进行安装。

### 利用原始环境中的pip list,构建一个新的python环境
利用pip list导出的列表，再加上正则表达式的过滤来最终生成所需要的requirements.txt.注意一般的话直接 == 所需版本号很可能会出错，一般修改成 >= 。
在安装时需要注意python安装中的包冲突问题，对需要安装的三方包而言，不同的包又依赖与其他三方包，这样如果都依赖于同一个其他三方包，且版本号存在冲突，会导致包的冲突，==注意此类情况==。

