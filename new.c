/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aitaouss <aitaouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 21:31:06 by aitaouss          #+#    #+#             */
/*   Updated: 2024/09/17 23:40:19 by aitaouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void    err(char *str)
{
    int i = -1;

    while (str[++i])
        write(2, &str[i], 1);
}

void    cd(char **argv, int i)
{
    if (i != 2)
        return err("error: cd: bad arguments\n");
    if (chdir(argv[1]) == -1)
        return err("error: cd: cannot change directory to "), err(argv[1]), err("\n");
}

void    exec(char **argv, char **envp, int i)
{
    int fd[2];
    int has_pipe = argv[i] && !strcmp(argv[i], "|");

    if (!has_pipe && strcmp(argv[i], "cd"))
        return cd(argv, i);
    
    if (has_pipe && pipe(fd) == -1)
        return err("error: fatal\n");

    int pid = fork();
    if (!pid)
    {
        if (has_pipe && (dup2(fd[1], 1) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
            return err("error: fatal\n");
        if (!strcmp(*argv, "cd"))
            return cd(argv, i);
        execve(*argv, argv, envp);
        return
    }
}   

int main(int argc, char **argv, char **envp)
{
    int i = 0;

    if (argc > 1)
    {
        while (argv[i] && argv[++i])
        {
            argv += i;
            i = 0;
            while (argv[i] && strcmp(argv[i], "|") && strcmp(argv[i], ";"))
            {
                i++;
            }
            if (i)
                exec(argv, envp, i);
        }
    }
    return 0;
}