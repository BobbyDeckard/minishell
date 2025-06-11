#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>
# include <sys/types.h>
# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

// États du shell
typedef enum e_shell_state {
    INTERACTIVE,
    EXECUTING,
    HEREDOC_MODE
} t_shell_state;


// Variable globale unique (contrainte du sujet)
extern volatile sig_atomic_t g_signal_received;

typedef struct s_shell_data
{
	char	**envp;
	char	**paths;
	int		exit_status;
	t_shell_state	state; 
}	t_shell_data;

/* PROTOTYPES SIGNAUX */
void	signal_handler(int signum);
void	setup_interactive_signals(void);
void	setup_execution_signals(void);
void	setup_child_signals(void);
void	handle_signal_in_context(t_shell_data *shell);
void	handle_sigint_interactive(void);
void	handle_sigint_executing(void);

#endif