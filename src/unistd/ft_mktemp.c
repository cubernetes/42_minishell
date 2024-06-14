#include "libft.h"

#include <fcntl.h>
#include <unistd.h>

static char	translate_to_alnum(char c_)
{
	unsigned char	c;

	c = (unsigned char)c_;
	if (ft_isalnum(c))
		return ((char)c);
	else if (c <= 25)
		return ((char)c + 65);
	else if (26 <= c && c <= 47)
		return ((char)c + 71);
	else if (58 <= c && c <= 64)
		return ((char)c - 10);
	else if (91 <= c && c <= 96)
		return ((char)c + 6);
	return (translate_to_alnum((char)(c - 50)));
}

static void	mktemp_error(const char *fmt, ...)
{
	va_list	ap;

	va_start(ap, fmt);
	ft_vdprintf(STDERR_FILENO, ft_strjoin(fmt, "\n"), ap);
	va_end(ap);
}

char	*ft_mktemp(char *prefix)
{
	int			rd_fd;
	char		*random_bytes;
	static int	heredoc_idx;
	int			idx;
	char		*str;

	random_bytes = ft_malloc(sizeof(*random_bytes) * (ft_strlen(prefix) + 16));
	ft_strlcpy((char *)random_bytes, ft_strjoin("/tmp/", prefix),
		6 + ft_strlen(prefix));
	rd_fd = open("/dev/urandom", O_RDONLY);
	if (read(rd_fd, random_bytes + 5 + ft_strlen(prefix), 10) != 10)
	{
		close(rd_fd);
		mktemp_error("ft_mktemp: couldn't gather randomness from /dev/urandom, "
			"creating heredoc file with name '/tmp/tmp.heredoc%d'",
			heredoc_idx);
		str = ft_strjoin("heredoc", ft_itoa(heredoc_idx++));
		ft_strlcpy(random_bytes + 5 + ft_strlen(prefix),
			str, ft_strlen(str) + 1);
		return (ft_strdup(random_bytes));
	}
	close(rd_fd);
	idx = (int)ft_strlen(prefix) + 4;
	while (++idx < (int)ft_strlen(prefix) + 15)
		random_bytes[idx] = translate_to_alnum(random_bytes[idx]);
	random_bytes[ft_strlen(prefix) + 15] = 0;
	return (ft_strdup(random_bytes));
}
