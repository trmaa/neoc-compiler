"
" THIS CODE IS AI SLOP, I'M NOT RESPONSIBLE FOR WHAT HAPPENS!!!!
"

if exists("b:current_syntax")
    finish
endif

" Clear any existing syntax first
syntax clear

" ONLY COMMENTS - nothing else
syntax match neoc_comment "//.*$"
syntax region neoc_comment start="/\*" end="\*/"

" Types
syntax keyword neoc_type let const static
syntax keyword neoc_type i8 u8 i16 u16 i32 u32 i64 u64 f32 f64 int float void char double long struct enum bool FILE clock_t short
syntax match neoc_type "%."
syntax match neoc_type "\\."

" Functions
syntax keyword neoc_keyword typedef sizeof system pub if start in else while for return fn continue break foreach switch case default cfor goto
syntax match neoc_functionCall "\k\+\s*\ze("
syntax match neoc_functionName "\<fn\s\+\zs\k\+" contained

" Strings
syntax region neoc_string start=+"+ end=+"+ skip=+\\"+
syntax region neoc_char start=+'+ end=+'+ skip=+\\'+

" Numbers
syntax match neoc_number "\<\d\+\>"
syntax match neoc_number "\<\d\+\.\d\+\>"
syntax match neoc_number "\<0b[01]\+\>"
syntax match neoc_number "\<0x[0-9a-fA-F]\+\>"
syntax match neoc_number "\<[A-Z_A-Z]\+\>"
syntax keyword neoc_number true false NULL EOF EXIT_FAILURE EXIT_SUCCESS

" Preprocessor directives
syntax match neoc_preproc "^#.*$"
syntax match neoc_preproc "use .*$"
syntax match neoc_preproc "def .*$"

highlight default link neoc_comment Comment
highlight default link neoc_keyword Keyword
highlight default link neoc_type Type
highlight default link neoc_string String
highlight default link neoc_char Character
highlight default link neoc_number Number
highlight default link neoc_operator Operator
highlight default link neoc_functionName Function
highlight default link neoc_functionCall Function
highlight default link neoc_preproc PreProc

let b:current_syntax = "neoc"
