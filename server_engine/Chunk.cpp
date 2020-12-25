/* *************************************************************************************/
/*                                                                                     */
/*                                                             |\---/|                 */
/*  Chunk.cpp                                                  | o_o |                 */
/*                                                             ‾‾‾‾‾‾‾                 */
/*  By: deddara <deddara@student-21.school.ru>                 ┌┬┐┌─┐┌┬┐┌┬┐┌─┐┬─┐┌─┐   */
/*                                                             _││├┤  ││ ││├─┤├┬┘├─┤   */
/*  created: 12/25/20 22:50:02 by deddara                      ─┴┘└─┘─┴┘─┴┘┴ ┴┴└─┴ ┴   */
/*  updated: 12/26/20 00:04:09 by deddara                      +-++-++-++-++-++-++-+   */
/*                                                             |)[-|)|)/-\|2/-\        */
/*                                                                                     */
/* **********************************************************²**************************/
#include "Chunk.hpp"

const int & Chunk::getCount(void){ return count; }
const int & Chunk::getLenSum(void) { return len_sum; }
const int & Chunk::getLen(void) { return len; }

void	Chunk::setLenSum(const int &num) { len_sum = num; }
void	Chunk::setLen(const int &num) { len = num; }
void	Chunk::setCount(const int &num) { count = num; }

int		Chunk::takeNum(const char *src,const int &num){
	return (0);
}
