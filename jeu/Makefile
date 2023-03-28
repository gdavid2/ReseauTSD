systeme=`hostname -s`

GCC=cc

all: tcp udp

tcp: serveur_TCP client_TCP
udp: serveur_UDP client_UDP

serveur_TCP:serveur_TCP.c commun.h
	$(GCC) -o $@-$(systeme) serveur_TCP.c

client_TCP:client_TCP.c commun.h
	$(GCC) -o $@-$(systeme) client_TCP.c

serveur_UDP:serveur_UDP.c commun.h
	$(GCC) -o $@-$(systeme) serveur_UDP.c

client_UDP:client_UDP.c commun.h
	$(GCC) -o $@-$(systeme) client_UDP.c
	
serveur_UDP:serveur_UDP.c jeu.h
	$(GCC) -o $@-$(systeme) serveur_UDP.c

client_UDP:client_UDP.c jeu.h
	$(GCC) -o $@-$(systeme) client_UDP.c



clean: clean-TCP clean-UDP

clean-TCP:
	rm -f serveur_TCP-$(systeme) client_TCP-$(systeme)
clean-UDP:
	rm -f serveur_UDP-$(systeme) client_UDP-$(systeme)
