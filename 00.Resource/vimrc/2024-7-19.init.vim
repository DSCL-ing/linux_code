"""""""""""""""""""""""""" vim-plug 插件管理器
" PlugInstall 安装插件
" PlugUpgrade 更新 vim-plug 自身
call plug#begin('~/.vim/bundle')
Plug 'Valloric/YouCompleteMe'
" Plug 'bling/vim-airline'
" Plug 'vim-airline/vim-airline-themes'
" Plug 'morhetz/gruvbox'
" Plug 'octol/vim-cpp-enhanced-highlight'
" Plug 'SirVer/ultisnips'
" Plug 'tpope/vim-surround'
" Plug 'flazz/vim-colorschemes'
" Plug 'scrooloose/nerdtree'
" Plug 'python-mode/python-mode'
" Plug 'scrooloose/nerdcommenter'
" Plug 'Yggdroot/LeaderF'
" Plug 'cpiger/NeoDebug'
" Plug 'ryanoasis/vim-devicons'
" Plug 'autozimu/LanguageClient-neovim', {
"    \ 'branch': 'next',
"    \ 'do': 'bash install.sh',
"    \ }
" Plug 'fatih/vim-go'
call plug#end()















"""""""""""""""""""""""""" 零碎配置

" 选中的文本在终端模式下显示为黄色背景和黑色文本 hi==higtlight
" 231:白
highlight Visual ctermfg=none ctermbg=yellow

"vim关闭所有鼠标操作
set mouse=

"系统剪切板+
set clipboard+=unnamedplus 

"设置doc手册文档为中文版,不过没什么用,nvim没有官方中文手册
set helplang=cn


" 设置vim下方状态栏
" set statusline=%FS
" set statusline=%F%m%r%h%w%=\ %{\"[fenc=\".(&fenc==\"\"?&enc:&fenc).((exists(\"+bomb\")\ &&\ &bomb)?\"+\":\"\").\"]\"}\ [pos=%04l,%04v][%p%%]
" set statusline=%F%m%r%h%w%i%s%t%m%
" set statusline=
set statusline+=%F                " 文件名
set statusline+=%h                " 帮助文件标志
set statusline+=%m                " 修改标志
set statusline+=%r                " 只读标志
" set statusline+=\ %{mode()}     " 当前模式
" set statusline+=%y              " 文件类型
" set statusline+=%{&ff}          " 文件格式
set statusline+=%=                " 分隔符（左对齐）
set statusline+=\ [%{&fenc}]      " 文件编码
set statusline+=%p%%              " 文件位置百分比
set statusline+=%l:%c             " 行数和列数
" 2始终显示状态栏 1多个窗口才显示 0不显示
set laststatus=2 



nmap <CR> i<CR><Esc>

""" 其他
" 调整窗口移动
" nnoremap H <C-w>h
" nnoremap J <C-w>j
" nnoremap K <C-w>k
" nnoremap L <C-w>l

" 快速保存
inoremap jk <esc>:w<cr>
nnoremap <esc> :w<cr>
inoremap <esc> <esc>:w<cr>



" 快速缩进
vnoremap < <gv
vnoremap > >gv


" set viminfo+='1000
" 保存光标位置
autocmd BufReadPost * if line("'\"") > 1 && line("'\"") <= line("$") | exe "normal! g'\"" | endif


"禁止生成swap文件
set noswapfile 

"高亮搜索
set hlsearch

let mapleader = ";" 


" 设置字符为双宽度(适合东亚语言),single为单宽度
set ambiwidth=double

set nu

syntax on "语法高亮

" Smart Indent 智能缩进(识别语法)
set si
" Auto Indent 自动缩进
set ai
" 两者搭配使用

" 不自动创建备份文件
set nowritebackup

" 智能删除Baskspace功能,不过不需要,nvim中目前没发现有什么用
set backspace=indent,eol,start

" expand:扩展/增强, 将tab插入制表符改成空格
set expandtab
" 设置tab所占的宽度(单位是空格数)
set tabstop=2
" 设置自动缩进所使用的空格数
set shiftwidth=2
" 插入模式下,tab实际插入的宽度(单位是空格数)
set softtabstop=2

" 文件编码的识别顺序,指定识别文件编码的优先级列表(如果都没有则默认以utf8)
set fileencodings=utf-8,gbk,cp936,cp950,latin1

" 设置新建文件的编码为utf-8
set encoding=utf-8

" 显示当前光标所在的行号、列号等信息
set ruler

" 回显输入的命令,在状态栏的下面
set showcmd


" 增量搜索（Incremental Search）是一种文本搜索方法，它在你输入搜索模式的同时就开始查找匹配项。也就是说，每当你在搜索框中输入一个字符，程序就会立即在文档中寻找以当前输入的字符序列开头的所有匹配项。
" 开启增量搜索, /xxx 或 ?xxx 就是搜索方式
set incsearch
" 开启搜索大小写忽略
set ignorecase
" 智能覆盖大小写忽略,当输入大写时,则会区分大小写
set smartcase


" zc	关闭当前打开的折叠
" zo	打开当前的折叠
" zm	关闭所有折叠
" zM	关闭所有折叠及其嵌套的折叠
" zr	打开所有折叠
" zR	打开所有折叠及其嵌套的折叠
" zd	删除当前折叠
" zE	删除所有折叠
" zj	移动至下一个折叠
" zk	移动至上一个折叠
" zn	禁用折叠
" zN	启用折叠


" 代码折叠
" set fdm = indent
set foldmethod=syntax
" 启用折叠
set foldenable
" 启用行号左边,显示折叠的数量,丑,不用
" set foldcolumn=2
" 打开文件时代码的展开等级,越高展开越多,启用折叠后代码默认是折叠的,需要设置成自己需要的状态,99比较大,基本都能展开
set foldlevel=99
" 折叠时显示的信息:此处为显示代码首行
" set foldtext=getline(v:foldstart)
" filetype plugin on



" 以下三个好像没什么用,ssh不支持吧大概是,可以在真linuxOS下试试
" noerrorbells 关闭错误叫声
" set noeb
" visualbell 启动错误屏幕闪烁
" set vb
" 完全关闭错误提醒
" set t_vb=

" 高亮光标所在行
set cursorline

" 文件修改之后自动载入
set autoread

"在上下移动光标时，光标的上方或下方至少会保留显示的行数--屏幕滚动
set scrolloff=10  


"左下角不显示当前vim模式
" set noshowmode 
set showmode

"相对行号"
set relativenumber 

" if has("autocmd")
"   au BufReadPost * if line("'\"") > 1 && line("'\"") <= line("$") | exe "normal! g'\"" | endif
" endif








"""""""""""""""""""""""""" YouCompleteMe
set completeopt=longest,menu
autocmd InsertLeave * if pumvisible() == 0|pclose|endif

let g:ycm_global_ycm_extra_conf = '~/.ycm_extra_conf.py'
let g:ycm_confirm_extra_conf = 0
" let g:ycm_key_invoke_completion='<C-j>' //触发补全?
let g:ycm_error_symbol = 'E>'
let g:ycm_warning_symbol = 'W>'
" 使用 NerdFont 中的图标表示错误和警告
" let g:ycm_error_symbol = '﯇'
" let g:ycm_warning_symbol = '卵'
let g:ycm_enable_diagnostic_signs = 1
let g:ycm_enable_diagnostic_highlighting = 1
let g:ycm_collect_identifiers_from_comments_and_strings = 0
let g:ycm_complete_in_comments = 1
let g:ycm_complete_in_strings = 1
let g:ycm_min_num_of_chars_for_completion = 2
let g:ycm_python_binary_path = 'python'
let g:ycm_semantic_triggers =  {
      \ 'c,cpp,python,java,go,erlang,perl': ['re!\w{2}'],
      \ 'cs,lua,javascript': ['re!\w{2}'],
      \}


"""""""""""""""""""""""""" Tagbar(函数列表)
 let g:tagbar_ctags_bin='/usr/bin/ctags'
 let g:tagbar_width=20
 let g:tagbar_left=1
 let g:tagbar_sort = 0


"""""""""""""""""""""""""" NERDTree(文件列表)
 let g:NERDTreeWinPos="right"
 let g:NERDTreeWinSize=20


"""""""""""""""""""""""""" CtrlP(快速查找文件)
" let g:ctrlp_working_path_mode='a'


"""""""""""""""""""""""""" cpp-enhanced-highlight(c++ 语法高亮)
" 高亮类，成员函数，标准库和模板
let g:cpp_class_scope_highlight = 1
let g:cpp_member_variable_highlight = 1
let g:cpp_concepts_highlight = 1
let g:cpp_experimental_simple_template_highlight = 1
" 文件较大时使用下面的设置高亮模板速度较快，但会有一些小错误
let g:cpp_experimental_template_highlight = 0


"""""""""""""""""""""""""" UltiSnips(代码模板)
let g:UltiSnipsExpandTrigger="<C-j>"
let g:UltiSnipsListSnippets="<C-e>"
let g:UltiSnipsJumpForwardTrigger="<C-j>"
let g:UltiSnipsJumpBackwardTrgger="<C-k>"


"""""""""""""""""""""""""" LeaderF
let g:Lf_WindowHeight = 0.30


"""""""""""""""""""""""""" cquery
let g:LanguageClient_serverCommands = {
  \ 'cpp': ['~/.VimForCpp/cquery/bin/cquery', '--log-file=/tmp/cquery/cq.log'],
  \ 'cxx': ['~/.VimForCpp/cquery/bin/cquery', '--log-file=/tmp/cquery/cq.log'],
  \ 'hpp': ['~/.VimForCpp/cquery/bin/cquery', '--log-file=/tmp/cquery/cq.log'],
  \ 'cc': ['~/.VimForCpp/cquery/bin/cquery', '--log-file=/tmp/cquery/cq.log'],
  \ 'c': ['~/.VimForCpp/cquery/bin/cquery', '--log-file=/tmp/cquery/cq.log'],
  \ 'h': ['~/.VimForCpp/cquery/bin/cquery', '--log-file=/tmp/cquery/cq.log'],
\ }

let g:LanguageClient_loadSettings = 1
let g:LanguageClient_settingsPath = '/tmp/cquery/settings.json'

" 关闭 cquery 的诊断信息, 使用 Ycm 的足矣
let g:LanguageClient_diagnosticsEnable=0

" 指定项目的根目录标记.



""""""""""""""""""""""" PyMode
let g:pymode_lint_cwindow = 0
let g:pymode_lint_ignore = ["E305", "E501", "E302", "E391", "W391", "W605",]


""""""""""""""""""""""" 按键映射和快捷键提示
" 跳转到定义 gD
nnoremap <c-k> :YcmCompleter GoToDefinitionElseDeclaration<CR>  
" 返回到跳转前的位置
nnoremap <c-l> <c-o>


