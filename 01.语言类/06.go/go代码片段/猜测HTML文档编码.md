
### 1.1.猜测html文档的编码：
```go
golang.org/x/net/html
```
```go
encoding, _, _ := charset.DetermineEncoding(bytes, "")
```