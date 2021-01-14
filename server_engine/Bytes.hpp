/* *************************************************************************************/
/*                                                                                     */
/*                                                             |\---/|                 */
/*  Bytes.hpp                                                  | o_o |                 */
/*                                                             ‾‾‾‾‾‾‾                 */
/*  By: deddara <deddara@student-21.school.ru>                 ┌┬┐┌─┐┌┬┐┌┬┐┌─┐┬─┐┌─┐   */
/*                                                             _││├┤  ││ ││├─┤├┬┘├─┤   */
/*  created: 12/25/20 23:30:46 by deddara                      ─┴┘└─┘─┴┘─┴┘┴ ┴┴└─┴ ┴   */
/*  updated: 12/26/20 19:21:35 by deddara                      +-++-++-++-++-++-++-+   */
/*                                                             |)[-|)|)/-\|2/-\        */
/*                                                                                     */
/* **********************************************************²**************************/
#ifndef BYTES_H
# define BYTES_H

class Bytes{
private:
	unsigned long bytes_count;
	unsigned long prev_bytes_count;

public:
	Bytes();

	const unsigned long & getBytes() const;
	const unsigned long & getPrevBytes() const;
	void prevBytesCount(const int &num);
	void setBytes(const unsigned long &);
	void setPrevBytes(const unsigned long &);
	void bytesCount(const int &);

	char* bytesDup(char*, const char*,int const & len);
	char * bytesJoin(char*, const char*,int const & len, int const & src_len);
	char * bytesCut(char*, unsigned long const &);
};

#endif