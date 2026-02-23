use "buff.nh"

fn clear_buff(buff[BUFF_LEN]: char) ~void {
	for let i in 0..BUFF_LEN
		buff[i] = 0;
}
