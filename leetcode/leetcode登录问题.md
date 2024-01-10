# 在登陆vscode登录时，很多时候不能sign in。
这时需要手动登录网页，获取network，xhr，再f5刷新页面，获取左边event，里的request header的cookie来，再在vscode端进行cookie的登录就绪。
还有sign in的用户名实际上是，leetcode个性域名的最后一部分，而不一定是用户名称。