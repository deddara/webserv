/* *************************************************************************************/
/*                                                                                     */
/*                                                             |\---/|                 */
/*  Chunk.hpp                                                  | o_o |                 */
/*                                                             ‾‾‾‾‾‾‾                 */
/*  By: deddara <deddara@student-21.school.ru>                 ┌┬┐┌─┐┌┬┐┌┬┐┌─┐┬─┐┌─┐   */
/*                                                             _││├┤  ││ ││├─┤├┬┘├─┤   */
/*  created: 12/25/20 22:36:08 by deddara                      ─┴┘└─┘─┴┘─┴┘┴ ┴┴└─┴ ┴   */
/*  updated: 12/25/20 23:31:19 by deddara                      +-++-++-++-++-++-++-+   */
/*                                                             |)[-|)|)/-\|2/-\        */
/*                                                                                     */
/* **********************************************************²**************************/
#ifndef CHUNK_HPP
# define CHUNK_HPP
# include <iostream>

class Chunk{

private:
	int count;
	int len_sum;
	int len;

public:
	Chunk() : count(0), len_sum(0), len(0) {};
	~Chunk() {};

	const int & getCount(void);
	const int & getLenSum(void);
	const int & getLen(void);

	void	setLenSum(const int &);
	void	setLen(const int &);
	void	setCount(const int &);


};

#endif