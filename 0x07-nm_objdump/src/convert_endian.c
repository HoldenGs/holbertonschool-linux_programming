#include "../inc/readelf.h"


/**
 * convert_big_endian - convert from big endian to little endian format
 *
 * @data: data to convert
 * @size: size of data in bytes
 *
 * Return: converted data, or 0 if size isn't convertable
 */
uint64_t convert_big_endian(uint64_t data, int size)
{
	if (size == 2)
		return (be16toh(data));
	if (size == 4)
		return (be32toh(data));
	if (size == 8)
		return (be64toh(data));
	return (0);
}

/**
 * convert_little_endian - convert to little endian format (do nothing)
 *
 * @data: data to convert
 * @size: size of data (unused)
 *
 * Return: data
 */
uint64_t convert_little_endian(uint64_t data, int size)
{
	(void)size;
	return (data);
}
