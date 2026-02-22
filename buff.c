#define BUFF_LEN 256

void clear_buff(char buff[BUFF_LEN])
{
	for (int i = 0; i < BUFF_LEN; i++)
		buff[i] = 0;
}
