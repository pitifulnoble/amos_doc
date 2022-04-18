# Go中文和unicode字符转换

## 1.中文转unicode
```go
sText := "hello 你好"
textQuoted := strconv.QuoteToASCII(sText)
textUnquoted := textQuoted[1 : len(textQuoted)-1]
fmt.Println(textUnquoted)
```

```sh
hello \u4f60\u597d
```

## 2.unicode转中文
```go
import (
    "fmt"
    "strconv"
    "strings"
)

func unicodeToZn(raw []byte) ([]byte, error) {
    str, err := strconv.Unquote(strings.Replace(strconv.Quote(string(raw)), `\\u`, `\u`, -1))
    if err != nil {
        return nil, err
    }
    return []byte(str), nil
}
```