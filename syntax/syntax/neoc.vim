if exists("b:current_syntax")
	finish
endif

" keywords
syntax keyword neoc_keyword if start else while for use return fn let const EXIT_FAILURE EXIT_SUCCESS

" syntax keyword neoc_funcs main printf fprintf sprintf getc getchar scanf sscanf fscanf strcpy strcmp
syntax match neoc_functionCall "\k\+\s*\ze("
syntax match neoc_functionName "\<fn\s\+\zs\k\+" contained

syntax keyword neoc_type n8 z8 n16 z16 n32 z32 n64 z64 f32 f64 int float void char double long struct enum bool

" comments
syntax region neoc_comment start=/\/\// end=/\n/

" strings
syntax region neoc_string start=/"/ end=/"/ skip=/\\"/
syntax region neoc_char start=/'/ end=/'/ skip=/\\'/

" numbers
syntax match neoc_number "\<\d\+\>"
syntax match neoc_number "\<\d\+\.\d\+\>"

" operators
syntax match neoc_operator "[+\-*/%<>!&|]"

" set highlights
highlight default link neoc_keyword keyword
highlight default link neoc_type type
highlight default link neoc_comment comment
highlight default link neoc_string string
highlight default link neoc_char string
highlight default link neoc_number number
" highlight default link neoc_operator operator

highlight default link neoc_functionName Function
highlight default link neoc_functionCall Function

let b:current_syntax = "neoc"
