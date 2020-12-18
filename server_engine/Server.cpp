#include "Server.hpp"
#include "includes.hpp"
#include "Request.hpp"

int Server::setup(std::string const & host, int const port){
	struct sockaddr_in addr;
	int listen_sock;

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = ft_htons(port);
	addr.sin_addr.s_addr = inet_addr(host.c_str());

	if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("listen_sock");
		return (500);
	}
	fcntl(listen_sock, F_SETFL, O_NONBLOCK);
	int opt = 1;
	setsockopt(listen_sock, SOL_SOCKET,  SO_REUSEADDR, &opt, sizeof(opt));
	if ((bind(listen_sock, (struct sockaddr*)& addr, sizeof(addr))) < 0)
	{
		perror("bind");
		return (500);
	}
	if ((listen(listen_sock, 10)) < 0){
		perror("listen_sock");
		return (500);
	}
	server_socks.push_back(listen_sock);
	return (0);
}

void Server::set_prepare()
{
	FD_ZERO(&readset);
	FD_ZERO(&writeset);
	for (std::vector<int>::iterator it = server_socks.begin(); it != server_socks.end(); ++it)
		FD_SET(*it, &readset);
	for (std::vector<Client*>::iterator it = client_session.begin(); it !=  client_session.end(); ++it){
		FD_SET((*it)->getFd(), &readset);
		if ((*it)->getResponse().getStr().length() != 0){
			FD_SET((*it)->getFd(), &writeset);
		}
		if ((*it)->getFd() > max_fd)
			max_fd = (*it)->getFd();
	}
}

void Server::recv_msg(std::vector<Client*>::iterator it){
	int n;
	char buff[2048];
	bzero(&buff, 2048);
	n = recv((*it)->getFd(), buff, sizeof(buff), MSG_TRUNC);
	if (n <= 0)
	{
		(*it)->setStatus(3);
		return;
	}
	//втыкаем в наш остаток от предыдущего клиентского чтения (если их было несколько)
	(*it)->buffAppend(buff);
	//прочитали и это коец запроса??? Тогда мы получили все данные, можем переходить к парсу. Единственное такое пока не работает с POST, можете подумать
	if ((*it)->getBuff().rfind("\r\n\r\n") != std::string::npos)
		(*it)->setStatus(1);
}

void Server::closeConnection(std::vector<Client*>::iterator it){
	close((*it)->getFd());
	client_session.erase(it);
}

int Server::launch() {
	//главный цикл жизни сервера (Желательно потом разбить на еще доп методы, это я сделаю сам)
	for (;;){
		max_fd = server_socks.back();
		//подготовка слушающих сетов для select
		set_prepare();
		//ждем шум в любом из файловых дескрипторов
		if (select(max_fd + 1, &readset, &writeset, NULL, NULL) < 0)
			return (1);
		//проходимся по всем ФДшникам наших виртуальных серверов и проверяем новые подключение
		for (std::vector<int>::iterator it = server_socks.begin(); it != server_socks.end(); ++it) {
			if (FD_ISSET(*it, &readset)) {
				int accept_sock;
				struct sockaddr_storage ss;
				socklen_t slen = sizeof(ss);
				if ((accept_sock = accept(*it, (struct sockaddr *) &ss, &slen)) < 0) {
					perror("accept");
					return (1);
				}
				fcntl(accept_sock, F_SETFL, O_NONBLOCK);
				//есть подключение? Хуярим новую клиентскую сессию
				client_session.push_back(new Client(accept_sock));
			}
		}
		// Проходимся по всем фдшникам клиентской сессии
		for (std::vector<Client*>::iterator it = client_session.begin(); it !=  client_session.end(); ++it) {
			//Кто-то постучался? Тогда обрабатываем
			if (FD_ISSET((*it)->getFd(), &readset))
			{
				//Статус 0(дефолт) - готов к чтению, Статус 1 - готов к парсу, Статус 2 - готов к Обработке (пока нигде не применяется), Статус 3 - закрываем сессию. Все кейсы описаны в Client.hpp (проставляются по дефолту от 0-n)
				switch ((*it)->getStatus()) {
					//получаем письмо
					case rdy_recv:
						recv_msg(it);
					case rdy_parse:
						// если статус не выход, парсим и выходим
						if ((*it)->getStatus() != 3)
						{
							//вот сюда в условия можно пихать генерацию респонса
							(*it)->getRequest()->req_init(((*it)->getBuff()));
							if ((*it)->getRequest()->error())
								(*it)->getResponse().response_prepare(1);
							else
								(*it)->getResponse().response_prepare(0);
							(*it)->setStatus(2);
							(*it)->clearBuff();
							break;

						}
						//закрываем коннект
					case finish:
						closeConnection(it);
						is_closed = 1;
						break;
				}

				//этот кусок говна надо улучшить (избавиться от флага что сессия умерла)
				if (is_closed)
				{
					is_closed = 0;
					break;
				}
//				if (recv_msg(it))
//					break;
//				Request req(static_cast<std::string>(buff));
//				if (req.error())
//				{
//					response_prepare_2();
//				}
//				else
//					response_prepare();
			}
			//если дескриптор готов к записи (пока не правильная версия, так как сенд может сработать и не за один раз. Надо будет делать такую же склейку как и у клиента с read_buff
			if (FD_ISSET((*it)->getFd(), &writeset)){
				if ((send((*it)->getFd(), (*it)->getResponse().getStr().c_str(),  (*it)->getResponse().getStr().length(), 0)) < 0)
				{
					perror("send");
					return 1;
				}
				//очищаем наш респонс и делаем сессию снова готовой к обработке сообщений.
				(*it)->getResponse().clearStr();
				(*it)->setStatus(0);
			}
		}
	}
}
