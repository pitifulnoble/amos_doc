### 1.2.转换其他编码为utf8编码
```go
golang.org/x/text
```
```go
utf8Reader := transform.NewReader(resp.Body, simplifiedchinese.GBK.NewDecoder())
```