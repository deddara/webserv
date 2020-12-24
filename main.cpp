/* *************************************************************************************/
/*                                                                                     */
/*                                                             |\---/|                 */
/*  main.cpp                                                   | o_o |                 */
/*                                                             ‾‾‾‾‾‾‾                 */
/*  By: deddara <deddara@student-21.school.ru>                 ┌┬┐┌─┐┌┬┐┌┬┐┌─┐┬─┐┌─┐   */
/*                                                             _││├┤  ││ ││├─┤├┬┘├─┤   */
/*  created: 12/24/20 22:37:25 by deddara                      ─┴┘└─┘─┴┘─┴┘┴ ┴┴└─┴ ┴   */
/*  updated: 12/24/20 22:38:07 by deddara                      +-++-++-++-++-++-++-+   */
/*                                                             |)[-|)|)/-\|2/-\        */
/*                                                                                     */
/* **********************************************************²**************************/
#include "Server.hpp"
#include "includes.hpp"
#include "ConfParser.hpp"
#include "VirtServer.hpp"

int main(int argc, char **argv)
{

	ConfParser		confPars(argc == 2 ? argv[1] : "./webserv.conf");
	Server	serv(confPars.getServer());

	serv.setup();

	//биндим айпишники
	//поднимаем сервер
	if (serv.launch())
		return 1;

	return (0);
}
