#include "Server.h"


bool Server::ProcessPacket(size_t clientIndex, std::shared_ptr<GNet::Packet> packet)
{
	Client& client = this->clients[clientIndex];
	if (packet->GetPacketType() == PacketType::Request)
	{
		try
		{
			RequestPacket requestPacket(*packet);
			if (requestPacket.message.length() > 0)
			{
				Logger::Write(client.username + " [" + client.ToString() + "]: " + requestPacket.message, Logger::Reason::Info);
				/*	std::string logStatement; Treba da se radi u posebnom threadu sa queueom, spusta performanse previse
					if (client.isLoggedIn == true)
						logStatement = "INSERT INTO logs(client_ip, client_id, log_type, log_message) VALUES(\""
						+ client.ToString() + "\"," + std::to_string(client.id) + ",0,\"" + requestPacket.message + "\");";
					else
						logStatement = "INSERT INTO logs(client_ip, log_type, log_message) VALUES("
						+ std::to_string(client.id) + ",0,\"" + requestPacket.message + "\");";
					this->database.Execute(logStatement.c_str());
					this->database.Finish(); */
				if (requestPacket.message[0] == '/') // komanda
				{
					Command command(requestPacket.message);
					if (command.GetCommand() == "disconnect")
						this->CloseConnection(clientIndex, DisconnectReason::Disconnect);

					if (client.isLoggedIn == true)
					{
						if (command.GetCommand() == "login")
						{
							ServerUpdatePacket serverUpdatePacket("[ERROR] You are already logged in, use /logout to logout.", COMMAND_ERROR_COLOR);
							std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
							client.outgoing.Push(responsePacket);
						}
						else if (command.GetCommand() == "logout")
						{
							this->OnLogout(clientIndex);
							client.username = "";
							client.lowerUsername = "";
							client.password = "";
							client.flag = ClientFlag::Default;
							client.isLoggedIn = false;
						}
						else if (command.GetCommand() == "kick")
						{
							if (command.GetArgumentCount() == 1)
							{
								if (client.flag >= ClientFlag::Moderator)
								{
									bool foundUser = false;
									for (size_t i = 0; i < this->clients.size(); i++)
									{
										if (this->clients[i].username == command.GetArgument(0) && this->clients[i].flag <= client.flag)
										{
											foundUser = true;
											this->CloseConnection(i, DisconnectReason::Kick);
											ServerUpdatePacket serverUpdatePacket("[SUCCESS] User kicked.", COMMAND_SUCCESS_COLOR);
											std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
											client.outgoing.Push(responsePacket);
											break;
										}
									}
									if (foundUser == false)
									{
										ServerUpdatePacket serverUpdatePacket("[ERROR] User not found or has flag above yours.", COMMAND_ERROR_COLOR);
										std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
										client.outgoing.Push(responsePacket);
									}
								}
								else
								{
									ServerUpdatePacket serverUpdatePacket("[ERROR] You may not use this command.", COMMAND_ERROR_COLOR);
									std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
									client.outgoing.Push(responsePacket);
								}
							}
							else
							{
								ServerUpdatePacket serverUpdatePacket("[ERROR] Wrong format, format: /kick <username>.", COMMAND_ERROR_COLOR);
								std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
								client.outgoing.Push(responsePacket);
							}
						}
						else if (command.GetCommand() == "adduser")
						{
							if (command.GetArgumentCount() == 3)
							{
								if (client.flag >= ClientFlag::Moderator)
								{
									if (Client::IsValidUsername(command.GetArgument(0)) && Client::IsValidPassword(command.GetArgument(1)))
									{

										std::string statement = "SELECT COUNT(*) FROM clients WHERE username=LOWER(\"" + command.GetArgument(0) + "\");";
										if (this->database.Execute(statement.c_str()) == SQLITE_ROW)
										{
											if (this->database.GetIntegerValue(0) > 0)
											{
												this->database.Finish();
												ServerUpdatePacket serverUpdatePacket("[ERROR] Username already registered.", COMMAND_ERROR_COLOR);
												std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
												client.outgoing.Push(responsePacket);
											}
											else
											{
												int32_t flag = -1;
												try { flag = stoi(command.GetArgument(2)); }
												catch (std::exception& ex)
												{
													ServerUpdatePacket serverUpdatePacket("[ERROR] Invalid flag given.", COMMAND_ERROR_COLOR);
													std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
													client.outgoing.Push(responsePacket);
												}
												if (flag < ClientFlag::Default || flag > ClientFlag::Admin)
												{
													ServerUpdatePacket serverUpdatePacket("[ERROR] Invalid flag given.", COMMAND_ERROR_COLOR);
													std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
													client.outgoing.Push(responsePacket);
												}
												else
												{
													if (flag <= client.flag)
													{
														statement = "INSERT INTO clients(username, password, flag) VALUES(LOWER(\"" + command.GetArgument(0) + "\"),\"" + command.GetArgument(1) + "\"," + command.GetArgument(2) + ");";
														if (database.Execute(statement.c_str()) == SQLITE_DONE)
														{ // konacno......
															ServerUpdatePacket serverUpdatePacket("[SUCCESS] User added.", COMMAND_SUCCESS_COLOR);
															std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
															client.outgoing.Push(responsePacket);
														}
														else
														{
															ServerUpdatePacket serverUpdatePacket("[ERROR] Failed updating the database.", COMMAND_ERROR_COLOR);
															std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
															client.outgoing.Push(responsePacket);
														}
														this->database.Finish();
													}
													else
													{
														ServerUpdatePacket serverUpdatePacket("[ERROR] You can't give bigger flag than yours.", COMMAND_ERROR_COLOR);
														std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
														client.outgoing.Push(responsePacket);
													}
												}
											}
										}
										else
										{
											this->database.Finish();
											ServerUpdatePacket serverUpdatePacket("[ERROR] Something went wrong.", COMMAND_ERROR_COLOR);
											std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
											client.outgoing.Push(responsePacket);
										}
									}
									else
									{
										ServerUpdatePacket serverUpdatePacket("[ERROR] Username or the password don't fit the server rules.", COMMAND_ERROR_COLOR);
										std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
										client.outgoing.Push(responsePacket);
									}
								}
								else
								{
									ServerUpdatePacket serverUpdatePacket("[ERROR] You may not use this command.", COMMAND_ERROR_COLOR);
									std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
									client.outgoing.Push(responsePacket);
								}
							}
							else
							{
								ServerUpdatePacket serverUpdatePacket("[ERROR] Wrong format, format: /adduser <username> <password> <flag>.", COMMAND_ERROR_COLOR);
								std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
								client.outgoing.Push(responsePacket);
							}
						}
						else if (command.GetCommand() == "removeuser")
						{
							if (command.GetArgumentCount() == 1)
							{
								if (client.flag >= ClientFlag::Moderator)
								{
									if (Client::IsValidUsername(command.GetArgument(0)))
									{
										std::string statement = "SELECT COUNT(*) FROM clients WHERE username=LOWER(\"" + command.GetArgument(0) + "\");";
										if (this->database.Execute(statement.c_str()) == SQLITE_ROW)
										{
											if (this->database.GetIntegerValue(0) > 0)
											{
												this->database.Finish();
												statement = "SELECT flag FROM clients WHERE username=LOWER(\"" + command.GetArgument(0) + "\");";
												if (this->database.Execute(statement.c_str()) == SQLITE_ROW)
												{
													if (this->database.GetIntegerValue(0) <= client.flag)
													{
														this->database.Finish();
														statement = "DELETE FROM clients WHERE username=LOWER(\"" + command.GetArgument(0) + "\");";

														if (this->database.Execute(statement.c_str()) == SQLITE_DONE)
														{
															this->database.Finish();
															ServerUpdatePacket serverUpdatePacket("[SUCCESS] User removed.", COMMAND_SUCCESS_COLOR);
															std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
															client.outgoing.Push(responsePacket);
														}
														else
														{
															this->database.Finish();
															ServerUpdatePacket serverUpdatePacket("[ERROR] Error deleting user from the database.", COMMAND_ERROR_COLOR);
															std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
															client.outgoing.Push(responsePacket);
														}
													}
													else
													{
														this->database.Finish();
														ServerUpdatePacket serverUpdatePacket("[ERROR] That user has a higher flag than you do.", COMMAND_ERROR_COLOR);
														std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
														client.outgoing.Push(responsePacket);
													}
												}
												else
												{
													this->database.Finish();
													ServerUpdatePacket serverUpdatePacket("[ERROR] Error executing db query.", COMMAND_ERROR_COLOR);
													std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
													client.outgoing.Push(responsePacket);
												}
											}
											else
											{
												this->database.Finish();
												ServerUpdatePacket serverUpdatePacket("[ERROR] Username doesn't exist.", COMMAND_ERROR_COLOR);
												std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
												client.outgoing.Push(responsePacket);
											}
										}
										else
										{
											this->database.Finish();
											ServerUpdatePacket serverUpdatePacket("[ERROR] Something went wrong.", COMMAND_ERROR_COLOR);
											std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
											client.outgoing.Push(responsePacket);
										}
									}
									else
									{
										ServerUpdatePacket serverUpdatePacket("[ERROR] Username doesn't fit server rules.", COMMAND_ERROR_COLOR);
										std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
										client.outgoing.Push(responsePacket);
									}
								}
								else
								{
									ServerUpdatePacket serverUpdatePacket("[ERROR] You may not use this command.", COMMAND_ERROR_COLOR);
									std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
									client.outgoing.Push(responsePacket);
								}
							}
							else
							{
								ServerUpdatePacket serverUpdatePacket("[ERROR] Wrong format, format: /adduser <username> <password> <flag>.", COMMAND_ERROR_COLOR);
								std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
								client.outgoing.Push(responsePacket);
							}
						}
						else if (command.GetCommand() == "help")
						{
							ServerUpdatePacket serverUpdatePacket("[HELP] /login | /logout | /kick | /adduser | /removeuser | /list | /help", COMMAND_SUCCESS_COLOR);
							std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
							client.outgoing.Push(responsePacket);
						}
						else if (command.GetCommand() == "list")
						{
							if (command.GetArgumentCount() == 0)
							{
								std::string response = "[LIST] Currently there are " + std::to_string(this->GetLoggedInCount())
									+ " user(s) online. Showing first 10:";
								for (size_t i = 0, count = 0; i < this->clients.size() && count < 10; i++)
								{
									if (this->clients[i].isLoggedIn == true)
									{
										response += (" " + this->clients[i].username);
										count++;
									}
								}
								ServerUpdatePacket serverUpdatePacket(response, COMMAND_SUCCESS_COLOR);
								std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
								client.outgoing.Push(responsePacket);
							}
							else if (command.GetArgumentCount() == 1)
							{
								ServerUpdatePacket serverUpdatePacket("[ERROR] Not implemented yet.", COMMAND_ERROR_COLOR);
								std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
								client.outgoing.Push(responsePacket);
							}
							else
							{
								ServerUpdatePacket serverUpdatePacket("[ERROR] Wrong format, format: /list <optional:page>.", COMMAND_ERROR_COLOR);
								std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
								client.outgoing.Push(responsePacket);
							}
						}
						else
						{
							ServerUpdatePacket serverUpdatePacket("[ERROR] Invalid command specified.", COMMAND_ERROR_COLOR);
							std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
							client.outgoing.Push(responsePacket);
						}
					}
					else
					{
						if (command.GetCommand() == "login" && command.GetArgumentCount() == 2)
						{
							std::string username = command.GetArgument(0);
							std::string password = command.GetArgument(1);

							std::string lowerUsername = username;
							//convert to lower https://stackoverflow.com/questions/313970/how-to-convert-stdstring-to-lower-case/24063783
							std::transform(lowerUsername.begin(), lowerUsername.end(), lowerUsername.begin(),
								[](unsigned char c) { return std::tolower(c); });

							if ((Client::IsValidUsername(username) == true) && (Client::IsValidPassword(password) == true))
							{
								if (this->IsLoggedIn(lowerUsername) == false)
								{
									std::string statement = "SELECT client_id, flag FROM clients WHERE username=\"" + lowerUsername + "\" AND password=\"" + password + "\";";
									if (this->database.Execute(statement.c_str()) == SQLITE_ROW)
									{
										client.username = username;
										client.password = password;
										client.lowerUsername = lowerUsername;
										client.isLoggedIn = true;
										client.id = this->database.GetIntegerValue(0);
										client.flag = static_cast<ClientFlag>(this->database.GetIntegerValue(1));
										this->OnLogin(clientIndex);
									}
									else
									{
										ServerUpdatePacket serverUpdatePacket("[ERROR] Invalid creditentals.", COMMAND_ERROR_COLOR);
										std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
										client.outgoing.Push(responsePacket);
									}
									this->database.Finish();
								}
								else
								{
									ServerUpdatePacket serverUpdatePacket("[ERROR] User is already logged in.", COMMAND_ERROR_COLOR);
									std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
									client.outgoing.Push(responsePacket);
								}
							}
							else
							{
								ServerUpdatePacket serverUpdatePacket("[ERROR] Invalid creditentals.", 0xFFFFFF);
								std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
								client.outgoing.Push(responsePacket);
							}
						}
						else
						{
							ServerUpdatePacket serverUpdatePacket("Please use /login <username> <password> to log in.", 0xFFFFFF);
							std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
							client.outgoing.Push(responsePacket);
						}
					}
				}
				else
				{
					if (client.isLoggedIn == true)
					{
						ChatMessagePacket messagePacket(client.username, requestPacket.message);
						this->SendAll(messagePacket.GeneratePacket());
					}
					else
					{
						ServerUpdatePacket serverUpdatePacket("Please use /login <username> <password> to log in.", 0xFFFFFF);
						std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(serverUpdatePacket.GeneratePacket());
						client.outgoing.Push(responsePacket);
					}
				}
			}
			else
			{
				Logger::Write("Empty packet was received from " + client.ToString() + ".", Logger::Reason::Warning);
				this->CloseConnection(clientIndex, DisconnectReason::Kick);
				return false;
			}
		}
		catch (GNet::PacketException& exception)
		{
			Logger::Write(exception.what(), Logger::Reason::Error);
			this->CloseConnection(clientIndex, DisconnectReason::Kick);
			return false;
		}
	}
	else
	{
		Logger::Write("A packet that isn't of type Request was received from " + client.ToString() + ".", Logger::Reason::Warning);
		this->CloseConnection(clientIndex, DisconnectReason::Kick);
		return false;
	}
	return true;
}

void Server::TryAcceptNewConnection()
{

	if (this->masterFds[0].revents & POLLIN)
	{

		try
		{
			GNet::IPEndpoint clientIPEndpoint;
			GNet::TCPSocket clientSocket = this->listeningSocket.Accept(clientIPEndpoint);
			clientSocket.SetBlocking(false);
			Client& client = this->clients.emplace_back(Client(clientSocket, clientIPEndpoint));
			pollfd newConnectionFD = {};
			newConnectionFD.fd = clientSocket.GetHandle();
			newConnectionFD.events = POLLIN;
			newConnectionFD.revents = 0;
			this->masterFds.push_back(newConnectionFD);
			Logger::Write(client.ToString() + " connected.", Logger::Reason::Info);
			if (this->maxClients < this->clients.size())
			{
				Logger::Write(client.ToString() + " kicked. Server is full.", Logger::Reason::Info);
				this->CloseConnection(this->clients.size() - 1, DisconnectReason::Kick);
			}
		}
		catch (GNet::SocketException exception) { Logger::Write(exception.what(), Logger::Reason::Error); }
	}
}

void Server::OnLogin(size_t clientIndex)
{
	Client& client = this->clients[clientIndex];
	Logger::Write(client.username + " [" + client.ToString() + "] logged in.", Logger::Reason::Info);
	ServerUpdatePacket serverUpdatePacket(client.username + " logged in.", CONNECTIONUPDATE_COLOR);
	this->SendAll(serverUpdatePacket.GeneratePacket());
}

void Server::OnLogout(size_t clientIndex)
{
	Client& client = this->clients[clientIndex];
	Logger::Write(client.username + " [" + client.ToString() + "] logged out.", Logger::Reason::Info);
	ServerUpdatePacket serverUpdatePacket(client.username + " logged out.", CONNECTIONUPDATE_COLOR);
	this->SendAll(serverUpdatePacket.GeneratePacket());
}

void Server::OnDisconnect(size_t clientIndex, DisconnectReason reason)
{
	Client& client = this->clients[clientIndex];
	if (reason == DisconnectReason::Disconnect)
	{
		Logger::Write("[" + client.ToString() + "] disconnected.", Logger::Reason::Info);
		if (client.isLoggedIn == true)
		{
			ServerUpdatePacket serverUpdatePacket(client.username + " disconnected.", CONNECTIONUPDATE_COLOR);
			this->SendAll(serverUpdatePacket.GeneratePacket());
		}
	}
	else if (reason == DisconnectReason::Timeout)
	{
		Logger::Write(client.username + " [" + client.ToString() + "] timed out.", Logger::Reason::Info);
		if (client.isLoggedIn == true)
		{
			ServerUpdatePacket serverUpdatePacket(client.username + " timed out.", CONNECTIONUPDATE_COLOR);
			this->SendAll(serverUpdatePacket.GeneratePacket());
		}
	}
	else if (reason == DisconnectReason::Kick)
	{
		Logger::Write(client.username + " [" + client.ToString() + "] got kicked.", Logger::Reason::Info);
		if (client.isLoggedIn == true)
		{
			ServerUpdatePacket serverUpdatePacket(client.username + " got kicked.", CONNECTIONUPDATE_COLOR);
			this->SendAll(serverUpdatePacket.GeneratePacket());
		}

	}
}

uint32_t Server::GetLoggedInCount() const
{
	uint32_t loggedInCount = 0;
	for (size_t i = 0; i < this->clients.size(); i++)
		if (this->clients[i].isLoggedIn == true)
			loggedInCount++;
	return loggedInCount;
}

void Server::CloseConnection(size_t clientIndex, DisconnectReason reason)
{
	this->OnDisconnect(clientIndex, reason);
	try { this->clients[clientIndex].Close(); }
	catch (GNet::SocketException exception) { Logger::Write(exception.what(), Logger::Reason::Error); };
	this->masterFds.erase(this->masterFds.begin() + (clientIndex + 1));
	this->clients.erase(this->clients.begin() + clientIndex);
}

void Server::SendAll(const GNet::Packet& packet)
{
	std::shared_ptr<GNet::Packet> responsePacket = std::make_shared<GNet::Packet>(packet);
	for (size_t i = 0; i < this->clients.size(); i++)
		if (this->clients[i].isLoggedIn == true)
			this->clients[i].outgoing.Push(responsePacket);
}

bool Server::IsLoggedIn(const std::string& lowerUsername)
{
	for (size_t i = 0; i < this->clients.size(); i++)
		if (this->clients[i].isLoggedIn == true && this->clients[i].lowerUsername == lowerUsername)
			return true;
	return false;
}

Server::Server(uint16_t port, size_t maxClients) :
	database("db.db"),
	ipEndpoint(GNet::IPv4Address::GetLocalIpAddress(), port),
	maxClients(maxClients)
{
	try { this->listeningSocket = GNet::TCPSocket(this->ipEndpoint.GetVersion()); }
	catch (GNet::SocketException exception) { Logger::Write(exception.what(), Logger::Reason::Error); };
}

bool Server::Initialize()
{
	Logger::Write("Initializing server.", Logger::Reason::Info);
	this->clients.clear();
	/*
	CREATE TABLE IF NOT EXISTS clients
(
	client_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	username TEXT NOT NULL,
	password TEXT NOT NULL,
	flag INTEGER NOT NULL
);
*/
	if (database.Execute("CREATE TABLE IF NOT EXISTS clients(client_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,username TEXT NOT NULL,password TEXT NOT NULL,flag INTEGER NOT NULL);")
		!= SQLITE_DONE)
		return false;
	database.Finish();
	/*CREATE TABLE IF NOT EXISTS logs
(
	log_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	client_id INTEGER,
	client_ip TEXT,
	log_type INTEGER NOT NULL,
	log_message TEXT NOT NULL,
	FOREIGN KEY(client_id) REFERENCES clients(client_id)
);*/
	if (database.Execute("CREATE TABLE IF NOT EXISTS logs(log_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,client_id INTEGER,client_ip TEXT,log_type INTEGER NOT NULL,log_message TEXT NOT NULL,FOREIGN KEY(client_id) REFERENCES clients(client_id)); ")
		!= SQLITE_DONE)
		return false;
	database.Finish();

	if (database.Execute("SELECT COUNT(*) FROM clients;")
		!= SQLITE_ROW)
		return false;
	if (database.GetIntegerValue(0) == 0)
	{
		database.Finish();
		std::string username;
		std::string password;
		Logger::Write("No users in database, please enter yourself. You'll be given admin privilege and be able to use /adduser <username> <password> to add new users.");
		do
		{
			Logger::Write("Enter your username (" + std::to_string(Client::USERNAME_MIN_LENGTH) + std::string("-") + std::to_string(Client::USERNAME_MAX_LENGTH) + std::string(" chars): "));
			std::cin >> username;
		} while (Client::IsValidUsername(username) == false);
		do
		{
			Logger::Write("Enter your password (" + std::to_string(Client::PASSWORD_MIN_LENGTH) + std::string("-") + std::to_string(Client::PASSWORD_MAX_LENGTH) + std::string(" chars): "));
			std::cin >> password;
		} while (Client::IsValidPassword(password) == false);
		std::string statement = "INSERT INTO clients(username, password, flag) VALUES(\"" + username + "\",\"" + password + "\"," + std::to_string(ClientFlag::Admin) + ");";
		int32_t sqliteResultCode;
		if ((sqliteResultCode = database.Execute(statement.c_str())) != SQLITE_DONE)
		{
			database.Finish();
			Logger::Write("Error while inserting first user into db, sqlite code: " + std::to_string(sqliteResultCode), Logger::Reason::Error);
			return false;
		}
		database.Finish();
	}
	else
		database.Finish();
	try { this->listeningSocket.Bind(this->ipEndpoint); }
	catch (GNet::SocketException exception)
	{
		Logger::Write(exception.what(), Logger::Reason::Error);
		this->Shutdown();
		return false;
	}
	Logger::Write("Starting to listen on the specified endpoint.", Logger::Reason::Info);
	try { this->listeningSocket.Listen(this->ipEndpoint, 133799); }
	catch (GNet::SocketException exception)
	{
		Logger::Write(exception.what(), Logger::Reason::Error);
		this->Shutdown();
		return false;
	}
	pollfd listenerPollFd;
	listenerPollFd.fd = this->listeningSocket.GetHandle();
	listenerPollFd.events = POLLIN;
	listenerPollFd.revents = 0;
	this->masterFds.push_back(listenerPollFd);
	this->listeningSocket.SetBlocking(false);
	return true;
}

bool Server::Present()
{
#ifdef _WIN32
	SetWindowTextA(GetConsoleWindow(), std::string("GC-Server (" + std::to_string(this->clients.size()) + "/" + std::to_string(this->maxClients) + ")").c_str());
#endif
	for (size_t i = 0; i < this->clients.size(); i++)
		if (this->clients[i].outgoing.HasPendingPackets())
			this->masterFds[i + 1].events = POLLIN | POLLOUT;

	if (poll(this->masterFds.data(), this->masterFds.size(), 1) > 0)
	{

		this->TryAcceptNewConnection();

		for (size_t i = this->masterFds.size() - 1; i >= 1; i--)
		{

			size_t clientIndex = i - 1;
			Client& client = this->clients[clientIndex];

			if (this->masterFds[i].revents & POLLERR)
			{
				this->CloseConnection(clientIndex, DisconnectReason::Timeout);
				continue;
			}

			if (this->masterFds[i].revents & POLLHUP)
			{
				this->CloseConnection(clientIndex, DisconnectReason::Disconnect);
				continue;
			}

			if (this->masterFds[i].revents & POLLNVAL)
			{
				this->CloseConnection(clientIndex, DisconnectReason::Timeout);
				continue;
			}

			if (this->masterFds[i].revents & POLLIN)
			{

				int32_t bytesReceived = 0;
				if (client.incoming.currentTask == GNet::PacketHandlerTask::ProcessSize)
					bytesReceived = recv(this->masterFds[i].fd, reinterpret_cast<char*>(&client.incoming.currentPacketSize) + client.incoming.currentPacketOffset, sizeof(uint16_t) - client.incoming.currentPacketOffset, 0);
				else
					bytesReceived = recv(this->masterFds[i].fd, client.buffer + client.incoming.currentPacketOffset, client.incoming.currentPacketSize - client.incoming.currentPacketOffset, 0);

				if (bytesReceived == 0)
				{
					this->CloseConnection(clientIndex, DisconnectReason::Disconnect);
					continue;
				}

				if (bytesReceived < 0)
				{
					if (GetLastError() != EAGAIN)
					{
						this->CloseConnection(clientIndex, DisconnectReason::Timeout);
						continue;
					}
				}

				if (bytesReceived > 0)
				{
					client.incoming.currentPacketOffset += bytesReceived;

					if (client.incoming.currentTask == GNet::PacketHandlerTask::ProcessSize)
					{
						if (client.incoming.currentPacketOffset == sizeof(client.incoming.currentPacketOffset))
						{
							client.incoming.currentPacketSize = ntohs(client.incoming.currentPacketSize);
							if (client.incoming.currentPacketSize > GNet::Packet::MAX_PACKET_SIZE)
							{
								Logger::Write(client.ToString() + " attempted sending packet bigger than MAX_PACKET_SIZE, attempted size: "
									+ std::to_string(client.incoming.currentPacketSize), Logger::Reason::Fatal);
								this->CloseConnection(clientIndex, DisconnectReason::Kick);
								continue;
							}
							client.incoming.currentPacketOffset = 0;
							client.incoming.currentTask = GNet::PacketHandlerTask::ProcessContent;
						}
					}
					else
					{
						if (client.incoming.currentPacketOffset == client.incoming.currentPacketSize)
						{
							std::shared_ptr<GNet::Packet> packet = std::make_shared<GNet::Packet>();
							packet->Resize(client.incoming.currentPacketSize);
							memcpy(packet->Buffer(), client.buffer, client.incoming.currentPacketSize);
							client.incoming.Push(packet);
						}
					}
				}
				continue;
			}

			if (this->masterFds[i].revents & POLLOUT)
			{
				while (client.outgoing.HasPendingPackets())
				{
					if (client.outgoing.currentTask == GNet::PacketHandlerTask::ProcessSize)
					{
						client.outgoing.currentPacketSize = static_cast<uint16_t>(client.outgoing.Front()->GetSize());
						uint16_t bigEndianPacketSize = htons(client.outgoing.currentPacketSize);
						int32_t bytesSent = send(this->masterFds[i].fd, reinterpret_cast<char*>(&bigEndianPacketSize) + client.outgoing.currentPacketOffset, sizeof(uint16_t) - client.outgoing.currentPacketOffset, 0);
						if (bytesSent > 0)
						{
							client.outgoing.currentPacketOffset += bytesSent;
						}

						if (client.outgoing.currentPacketOffset == sizeof(uint16_t))
						{
							client.outgoing.currentPacketOffset = 0;
							client.outgoing.currentTask = GNet::PacketHandlerTask::ProcessContent;
						}
						else
							break;
					}
					else
					{
						const void* bufferPtr = client.outgoing.Front()->Data();
						int bytesSent = send(this->masterFds[i].fd, reinterpret_cast<const char*>(bufferPtr) + client.outgoing.currentPacketOffset, client.outgoing.currentPacketSize - client.outgoing.currentPacketOffset, 0);
						if (bytesSent > 0)
							client.outgoing.currentPacketOffset += bytesSent;


						if (client.outgoing.currentPacketOffset == client.outgoing.currentPacketSize)
							client.outgoing.Pop();
						else
							break;

					}
				}
				if (!client.outgoing.HasPendingPackets())
				{
					this->masterFds[i].events = POLLIN;
				}
			}
		}
	}


	for (int i = this->clients.size() - 1; i >= 0; i--)
	{
		Client& client = this->clients[i];
		if (client.incoming.HasPendingPackets())
		{
			std::shared_ptr<GNet::Packet> frontPacket = client.incoming.Front();
			client.incoming.Pop();
			ProcessPacket(i, frontPacket);
		}
	}
	return true;
}

void Server::Shutdown()
{
	try { this->listeningSocket.Close(); }
	catch (GNet::SocketException exception)
	{
		Logger::Write(exception.what(), Logger::Reason::Error);
	}
}
