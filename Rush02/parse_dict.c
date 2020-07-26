/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_dict.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jkoers <jkoers@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/07/25 14:21:15 by jkoers        #+#    #+#                 */
/*   Updated: 2020/07/26 12:29:33 by jkoers        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include "parse_dict.h"
#include "ft_helpers.h"
#define MAX_FILE_SIZE 10485760

char		*read_dict(char *filename)
{
	char			*dict;
	int				fd;
	unsigned int	bytes_read;

	dict = malloc(MAX_FILE_SIZE);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		write(1, "error\n", 6);
	bytes_read = read(fd, dict, MAX_FILE_SIZE);
	dict[bytes_read] = '\0';
	close(fd);
	return (dict);
}

void		match_rule(char *dict, unsigned int *start_i, unsigned int *end_i)
{
	unsigned int i;

	i = *start_i;
	while (dict[i] != '\0' && !is_number(dict[i]))
		i++;
	i = *start_i;
	while (dict[i] != '\0' && is_number(dict[i]))
		i++;
	while (dict[i] != '\0' && dict[i] == ' ')
		i++;
	if (dict[i] != '\0' && dict[i] != ':')
	{
		i++;
		write(1, "error\n", 6);
	}
	while (dict[i] != '\0' && dict[i] != '\n')
		i++;
	*end_i = i;
}

/*
**	>= n_rules alloc
*/

t_name_rule	*malloc_for_rules(char *dict, unsigned int *n_rules)
{
	t_name_rule		*rules = NULL;
	unsigned int	start_i;
	unsigned int	end_i;

	start_i = 0;
	*n_rules = 0;
	while (1)
	{
		*n_rules += 1;
		match_rule(dict, &start_i, &end_i);
		if (dict[start_i] == '\0' || dict[end_i] == '\0')
			break ;
	}
	rules = malloc(*n_rules * sizeof(t_name_rule));
	return (rules);
}

void		set_rule(t_name_rule *rule, char *str)
{
	unsigned int	i;

	i = 0;
	while (is_number(str[i]))
		i++;
	rule->number = malloc(i + 1);
	ft_strncpy(rule->number, str, i);
	rule->number[i] = '\0';
	while (str[i] == ' ' || str[i] == ':')
		i++;
	str += i;
	i = 0;
	while (str[i] != '\n')
		i++;
	rule->name = malloc(i + 1);
	rule->name[i] = '\0';
	ft_strncpy(rule->number, str, i);
	rule->number_length = i;
}

t_name_rule	*parse_dict(char *filename, unsigned int *number_of_names)
{
	char			*dict = NULL;
	t_name_rule		*rules;
	unsigned int	start_i;
	unsigned int	end_i;
	unsigned int	n_rules;

	dict = read_dict(filename);
	rules = malloc_for_rules(dict, &n_rules);
	*number_of_names = n_rules;
	start_i = 0;
	while (n_rules > 0)
	{
		n_rules--;
		match_rule(dict, &start_i, &end_i);
		set_rule(rules + n_rules, dict + start_i);
	}
	free(dict);
	return (rules);
}
