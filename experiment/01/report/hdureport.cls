\ProvidesClass{hdureport}
\LoadClass[UTF8]{ctexart}
\RequirePackage{graphicx, xcolor, tikz, lmodern}
\RequirePackage{geometry, titlesec, fancyhdr,pgffor}
\RequirePackage{listings, cncolours, algorithm, algorithmicx, algpseudocode}
\RequirePackage{indentfirst, enumitem, zhnumber, hyperref}

% 设置版式
\linespread{1.2}
\geometry{top=1in,bottom=1in,left=1in,right=1in}

\usepackage{hyperref}
\hypersetup{
    colorlinks=true,
    linkcolor=black,
}
% 设置标题和字体格式
% \setCJKmainfont[ItalicFont={kaishu},BoldFont={kaishu}]{kaishu}
% \setmonofont{kaishu}
% \setCJKfamilyfont{kaishu}[AutoFakeBold=2.5]{kaishu}

\renewcommand\thesection{\zhnumber{\arabic{section}}、}
\renewcommand\thesubsection{\arabic{subsection}.}
\renewcommand\thesubsubsection{(\arabic{subsubsection})}
\setlist[enumerate,1]{label=(\arabic*)}
\setlist[enumerate,2]{label=\alph*.}
\titleformat*{\section}{\large\bfseries}
\titleformat*{\subsection}{\normalsize\bfseries}
\titleformat*{\subsubsection}{\normalsize}

% 代码格式和颜色定义
\definecolor{Light}  {HTML} {686868}
\definecolor{Steel}  {HTML} {888888}
\definecolor{Dark}   {HTML} {262626}
\definecolor{Blue}   {HTML} {4172A3}
\definecolor{Green}  {HTML} {3b7c50}
% \definecolor{DarkGreen} {HTML} {3ac9b0}
\definecolor{Brown}  {HTML} {A07040}
\definecolor{Red}    {HTML} {AD4D3A}
\definecolor{Violet} {HTML} {7040A0}
\definecolor{Gray}   {HTML} {B8B8B8}
\definecolor{Silver} {HTML} {E0E0E0}
\definecolor{White}  {HTML} {FFFFFF}
\definecolor{Orange} {HTML} {FF9933}
\definecolor{DarkGreen} {HTML} {008000}
\definecolor{DarkBlue} {HTML} {000080}
\definecolor{DarkPurple} {HTML} {800080}
\definecolor{DarkRed} {HTML} {800000}
\definecolor{DarkGray} {HTML} {808080}
\definecolor{LightGray} {HTML} {D3D3D3}
\definecolor{Pink} {HTML} {FFC0CB}
\definecolor{LightBlue} {HTML} {ADD8E6}
\definecolor{LightGreen} {HTML} {90EE90}
\definecolor{LightPurple} {HTML} {E6E6FA}
\definecolor{LightRed} {HTML} {FFB6C1}
\definecolor{LightOrange} {HTML} {FFA500}

\lstset{
    basicstyle          =   \zihao{-5}\ttfamily,
    backgroundcolor     =   \color[RGB]{245,245,244},
    numberstyle         =   \zihao{-5}\ttfamily,            % 行号的样式，小五号，tt等宽字体
    keywordstyle        =   \bfseries\color{Orange},          % 关键字风格
    commentstyle        =   \songti\slshape\color{DarkGray},           % 注释的风格
    stringstyle         =   \ttfamily\color{Green},         % 字符串风格
    columns             =   flexible,                        
    numbers             =   left,                           % 行号的位置在左边
    showspaces          =   false,                          % 是否显示空格
    showstringspaces    =   false,
    breaklines          =   true,                           % 自动换行，建议不要写太长的行
    captionpos          =   t,                              % 这段代码的名字所呈现的位置，t指的是top上面
    frame               =   single,                         % 显示边框
    rulecolor           =   \color{black},                  % 没有该color 换行之后边框将会是别的颜色
    % escapeinside        =   {\%*}{*)},
    extendedchars       =   false,
    }
\lstdefinestyle{C++}{
    morekeywords={alignas,continute,friend,register,true,alignof,decltype,goto,
    reinterpret_cast,try,asm,defult,if,return,typedef,auto,delete,inline,short,
    typeid,bool,do,int,signed,typename,break,double,long,sizeof,union,case,
    dynamic_cast,mutable,static,unsigned,catch,else,namespace,static_assert,using,
    char,enum,new,static_cast,virtual,char16_t,char32_t,explict,noexcept,struct,
    void,export,nullptr,switch,volatile,class,extern,operator,template,wchar_t,
    const,false,private,this,while,constexpr,float,protected,thread_local,
    const_cast,for,public,throw,std},
    emph={std::map,std::set,std::multimap,std::multiset,std::unordered_map,std::unordered_set,std::
    unordered_multiset,std::unordered_multimap,std::vector,std::string,std::list,std::deque,std::
    array,std::stack,std::forwared_list,std::iostream,std::memory,std::shared_ptr,std::unique_ptr,std::
    random,std::bitset,std::ostream,std::istream,std::cout,std::cin,std::endl,std::move,default_random_engine,
    uniform_int_distribution,iterator,algorithm,functional,bing,numeric,},
    emphstyle       =   \color{Violet}\bfseries, 
    % basewidth       =   0.52em,
    columns         =   fixed,  % 如果不加这一句，字间距就不固定，很丑，必须加
    language        =   C++,
}


% 信息
\newcommand\major[1]{\def\@major{#1}}
\newcommand\name[1]{\def\@name{#1}}
\newcommand\stuid[1]{\def\@stuid{#1}}
\newcommand\college[1]{\def\@college{#1}}
\renewcommand\date[1]{\def\@date{#1}}
\newcommand\lab[1]{\def\@lab{#1}}
\newcommand\course[1]{\def\@course{#1}}
\newcommand\instructor[1]{\def\@instructor{#1}}
\newcommand\grades[1]{\def\@grades{#1}}
\newcommand\expname[1]{\def\@expname{#1}}
\newcommand\exptype[1]{\def\@exptype{#1}}
\newcommand\partner[1]{\def\@partner{#1}}
\graphicspath{{figures/}}

% 页眉
\pagestyle{fancy}
\lhead{实验名称：\@expname}
\chead{姓名：\@name}
\rhead{学号：\@stuid}

% 下划线定义
\def\udl[#1]#2{
  \underline{\hbox to #1{\hfil#2\hfil}}}

% 首页头部
\newcommand{\makeheader}{
\begin{figure}[h]
  \begin{minipage}{0.4\linewidth}
    \centerline{\includegraphics[width=0.5\linewidth]{logo}}
  \end{minipage}%
  \begin{minipage}{0.3\linewidth}
    \centerline{\fontsize{36}{50}\CJKfamily{\textbf{\xeCJKsetup{CJKglue=\hskip 0.8pt}实验报告}}}
  \end{minipage}%
  \begin{minipage}{.3\linewidth}
    \raggedleft
    \begin{tabular*}{.8\linewidth}{ll}
      专业： & \udl[70pt]\@major   \\
      姓名： & \udl[70pt]\@name    \\
      学号： & \udl[70pt]\@stuid   \\
      日期： & \udl[70pt]\@date \\
      地点： & \udl[70pt]\@lab
    \end{tabular*}
  \end{minipage}
\end{figure}

\begin{table}[!htbp]
  \centering
  \begin{tabular*}{\linewidth}{llllll}
    课程名称： & \udl[150pt]\@course   & 指导老师： & \udl[50pt]\@instructor   & 成绩：        & \udl[50pt]\@grades \\
    实验名称： & \udl[150pt]\@expname  & 实验类型： & \udl[50pt]\@exptype      & 同组学生姓名： & \udl[50pt]\@partner
  \end{tabular*}
\end{table}
\thispagestyle{empty}

}

% 封面
\newcommand{\makecover}{
  \begin{center}
    \begin{center}
      \includegraphics[width=0.35\linewidth]{hdu-logo.png}
    \end{center}
    \vspace{1cm}
    \bfseries\huge{《\@course》}\\
    \vspace{0.5cm}
    \bfseries\huge{实验报告}
    \vspace{1.5cm}
    \begin{center}
      \Large
      \linespread{1}
      \centering
      \renewcommand\arraystretch{1.8}
      \begin{tabular}{cc}
        \bfseries{课\quad \quad 程：} & \@expname \\ \cline{2-2}
        \bfseries{学\quad \quad 院：}     & \@college \\ \cline{2-2}
        \bfseries{专\quad \quad 业：}     & \@major  \\ \cline{2-2}
        \bfseries{姓\quad \quad 名：}     & \@name   \\ \cline{2-2}
        \bfseries{学\quad \quad 号：}     & \@stuid  \\ \cline{2-2}
        \bfseries{老\quad \quad 师：} & \@instructor  \\ \cline{2-2}
        \bfseries{完成时间：} & \@date  \\ \cline{2-2}
      \end{tabular}
    \end{center}
    \vspace{1cm}
  \end{center}
  \thispagestyle{empty}
  \newpage
}
\endinput