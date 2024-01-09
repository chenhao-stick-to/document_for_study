# anocond环境
conda create 无法创建环境时，一般都是condarc出了问题。可以删除channels下的镜像源路径，修改以下；添加代理等。
# pip 
pip install -r requirements.txt  --proxy=http://10.252.1.248:8889
pip install出现了
WARNING: Retrying (Retry(total=2, connect=None, read=None, redirect=None, status=None)) after connection broken by 'NewConnectionError('<pip._vendor.urllib3.connection.HTTPConnection object at 0x7feb6b9d49d0>: Failed to establish a new connection: [Errno 101] Network is unreachable')': /pypi/simple/protobuf/错误。
更换镜像源无法解决，需要配置代理，如上解决。