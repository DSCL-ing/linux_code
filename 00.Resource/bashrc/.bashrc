# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# Uncomment the following line if you don't like systemctl's auto-paging feature:
# export SYSTEMD_PAGER=

# User specific aliases and functions
alias vim='/home/chj/.VimForCpp/nvim'
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/.VimForCpp/vim/bundle/YCM.so/el7.x86_64

PS1='\[\e[36m\][\u@\h \W \t]\$ \[\e[m\]'

alias cd='cdls'
cdls() {                                                              
    if [ $# -eq 0 ]; then    
        builtin cd ~ && ls    
    else    
        builtin cd "$@" && ls    
    fi    
}    
 alias ..='cd ..'   

alias la='ll -a'

 alias 1.3='cd /home/chj/git_repositories/linux_code/1.3basicIO'    
 alias 1.4='cd /home/chj/git_repositories/linux_code/1.4file_sys'    
 alias 1.5='cd /home/chj/git_repositories/linux_code/1.5lib_learn'    
 alias 1.6='cd /home/chj/git_repositories/linux_code/1.6pipe_learn'






alias vim='/home/chj/.VimForCpp/nvim'
