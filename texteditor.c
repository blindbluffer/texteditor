#include<gtk/gtk.h>
#include<stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include<ctype.h>
#include "spell.h"


GtkWidget *view;
int sockfds;
int sockfdc;
int portno;
struct hostent *server;
#define h_addr h_addr_list[0]
GtkWidget *text_view;
GtkWidget *search_entry, *replace_entry, *port_entry, *ip_entry, *key_entry;
#define CHAR_BUF 2000
GtkWidget *window;
GtkTextBuffer *buf;
GtkItemFactory *main_menu;
char *filename,*key = NULL,*keys = NULL;
GdkAtom sel_atom = GDK_SELECTION_CLIPBOARD;



#define SEPCHARS "* .,\"\n"
#define MENU_NEW 1
#define MENU_OPEN 2
#define MENU_SAVE 3
#define MENU_SAVE_AS 4
#define MENU_QUIT 5
#define MENU_CUT 6
#define MENU_COPY 7
#define MENU_PASTE 8
#define MENU_USE_CLIPBOARD 9
#define MENU_SELECT_ALL 10
#define MENU_HELP 14
#define MENU_ABOUT 15
#define MENU_SERVER_PORTNO 16
#define MENU_SERVER 17
#define MENU_SEND_SERVER 18
#define MENU_RECIEVE_SERVER 19
#define MENU_CLIENT_PORTNO 20
#define MENU_IP 21
#define MENU_CLIENT 22
#define MENU_SEND_CLIENT 23
#define MENU_RECIEVE_CLIENT 24
#define MENU_ENCRYPTION 25
#define MENU_DECRYPTION 26
#define MENU_KEY 27
#define MENU_SPELL 28


void show_about(void);
void servert(void);
void client(void);
void recieves(void);
void recievec(void);
void sends(void);
void sendc(void);
void pn(void);
void ip(void);
void encryption(void);
void decryption(void);
void ec_clicked(void);
void dc_licked(void);
void spell_clicked(void);

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void portno_clicked ()
{
  char *p;
  
  p = (gchar *)gtk_entry_get_text((GtkEntry *)port_entry);
portno = atoi(p);
}
void pn(void)
{
GtkWidget *win;
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *portno_button;
win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
gtk_window_set_title(GTK_WINDOW(win), "portno");

  vbox = gtk_vbox_new (FALSE, 2);
  gtk_container_add (GTK_CONTAINER (win), vbox);

  hbox = gtk_hbox_new (FALSE, 2);

  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  
  port_entry = gtk_entry_new();
  
  gtk_box_pack_start (GTK_BOX (hbox),port_entry, TRUE, TRUE, 0);

  portno_button = gtk_button_new_with_label ("portno");
  gtk_box_pack_start (GTK_BOX (hbox), portno_button, FALSE, FALSE, 0);
g_signal_connect(G_OBJECT (portno_button), "clicked",G_CALLBACK (portno_clicked), NULL);
  
gtk_widget_show_all(win);

}
void servert(void)
{
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;
    sockfds = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfds < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfds, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfds,5);
     clilen = sizeof(cli_addr);
sockfds = accept(sockfds, (struct sockaddr *) &cli_addr, &clilen); 
    if(sockfds < 0) 
          error("ERROR on accept");
}

void ip_clicked ()
{
  char *p;
  
  p = (gchar *)gtk_entry_get_text((GtkEntry *)ip_entry);
server = gethostbyname(p);
}
void ip(void)
{
GtkWidget *win;
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *ip_button;
win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
gtk_window_set_title(GTK_WINDOW(win), "portno");

  vbox = gtk_vbox_new (FALSE, 2);
  gtk_container_add (GTK_CONTAINER (win), vbox);

  hbox = gtk_hbox_new (FALSE, 2);

  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  
  ip_entry = gtk_entry_new();
  
  gtk_box_pack_start (GTK_BOX (hbox),ip_entry, TRUE, TRUE, 0);

  ip_button = gtk_button_new_with_label ("IP");
  gtk_box_pack_start (GTK_BOX (hbox), ip_button, FALSE, FALSE, 0);
g_signal_connect(G_OBJECT (ip_button), "clicked",G_CALLBACK (ip_clicked), NULL);
  gtk_widget_show_all(win);

}
void client(void)
{
    struct sockaddr_in serv_addr;
    sockfdc = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfdc < 0) 
        error("ERROR opening socket");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfdc,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
}

void sends(void)
{
char *fname;
     char buffer[1048576];
bzero(buffer,1024);
 FILE *f;
   int ch =0 ;
int lines = 0;
    char c;
GtkWidget *dialog = gtk_file_selection_new("Share File...");
	int resp = gtk_dialog_run(GTK_DIALOG(dialog));
	if(resp == GTK_RESPONSE_OK) 
        {
	    fname = g_strdup(gtk_file_selection_get_filename(GTK_FILE_SELECTION(dialog)));
	    gtk_widget_destroy(dialog);
	} 
	else 
        {
	    gtk_widget_destroy(dialog);
	}
f=fopen(fname,"r");
    while((c=getc(f))!=EOF)			
	{	
	if(c == '\n')	                       
		   lines++;	
	}
printf("%d",lines);
 bzero(buffer,1048576); 
	write(sockfds, &lines, sizeof(int));
rewind(f);

while(ch != lines+1)
                          {
		fscanf(f ,"%[^\n]%*c", buffer);
		write(sockfds,buffer,1048576);
                bzero(buffer,1048576);
            ch++;
      }
    fclose(f);
    /*close(sockfds);*/
	printf("The file was sent successfully");

}
void sendc(void)
{
char *fname;
     char buffer[1048576];
bzero(buffer,1024);
 FILE *f;
   int ch =0 ;
int lines = 0;
    char c;
GtkWidget *dialog = gtk_file_selection_new("Share File...");
	int resp = gtk_dialog_run(GTK_DIALOG(dialog));
	if(resp == GTK_RESPONSE_OK) 
        {
	    fname = g_strdup(gtk_file_selection_get_filename(GTK_FILE_SELECTION(dialog)));
	    gtk_widget_destroy(dialog);
	} 
	else 
        {
	    gtk_widget_destroy(dialog);
	}
f=fopen(fname,"r");
    while((c=getc(f))!=EOF)
	{	
		if(c == '\n')
		lines++;	
	} 
bzero(buffer,1048576);    
	write(sockfdc, &lines, sizeof(int));
rewind(f);
while(ch != lines+1)
                          {
		fscanf(f , "%[^\n]%*c" , buffer);
		write(sockfdc,buffer,1048576);
                bzero(buffer,1048576);
            ch++;
      }
    fclose(f);
    /*close(sockfdc);*/
printf("The file was sent successfully");

}

void recieves(void)
{
     char buffer[1048576];
int lines = 0;
char *fname;

 FILE *fp;
         int ch = 0;
GtkWidget *dialog = gtk_file_selection_new("File Name...");
	int resp = gtk_dialog_run(GTK_DIALOG(dialog));
	if(resp == GTK_RESPONSE_OK) 
        {
	    fname = g_strdup(gtk_file_selection_get_filename(GTK_FILE_SELECTION(dialog)));
	    gtk_widget_destroy(dialog);
	} 
	else 
        {
	    gtk_widget_destroy(dialog);
	}
            fp = fopen(fname,"a");            
            
		read(sockfds, &lines, sizeof(int));
bzero(buffer,1048576);
          while(ch != lines+1)
       	   {
        	 read(sockfds , buffer , 1048576); 
	   	 fprintf(fp , "%s\n" , buffer);
                 bzero(buffer,1048576);   
		 ch++;
	   }
     fclose(fp);
     /*close(sockfds);*/
printf("The file was received successfully\n");
}

void recievec(void)
{
     char buffer[1048576];
int lines = 0;
char *fname;

 FILE *fp;
         int ch = 0;
GtkWidget *dialog = gtk_file_selection_new("File Name...");
	int resp = gtk_dialog_run(GTK_DIALOG(dialog));
	if(resp == GTK_RESPONSE_OK) 
        {
	    fname = g_strdup(gtk_file_selection_get_filename(GTK_FILE_SELECTION(dialog)));
	    gtk_widget_destroy(dialog);
	} 
	else 
        {
	    gtk_widget_destroy(dialog);
	}
            fp = fopen(fname,"a");            
            
		read(sockfdc, &lines, sizeof(int));
bzero(buffer,1048576);
          while(ch != lines+1)
       	   {
        	 read(sockfdc , buffer , 1048576); 
	   	 fprintf(fp , "%s\n" , buffer);
                 bzero(buffer,1048576);   
		 ch++;
	   }
     fclose(fp);
     printf("The file was received successfully\n");
}

void show_about(void)
{

    GtkWidget *window1;
    GtkWidget *label;

    window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window1), "About Text Editor");
    gtk_window_set_default_size(GTK_WINDOW(window1), 300, 300);
  

    label = gtk_label_new ("Text Editor With Socket Programing\nShaurya Gairola\nSneheel Shivam\nShivank Srivastava\nShivam Prakash\n");
    
    gtk_container_add (GTK_CONTAINER (window1), label); 

    gtk_widget_show_all (window1);
}



void show_help(void)
{   
}
void ec_clicked ()
{
char *p;
  p = (gchar *)gtk_entry_get_text((GtkEntry *)key_entry);
   key = (char *)p;
   keys = (char *)p;
printf("%s", key);
printf("%s", keys);
char *fname;
int a;
 FILE *f1,*f2;
    char c;
printf("%s", key);
GtkWidget *dialog = gtk_file_selection_new("Share File...");
	int resp = gtk_dialog_run(GTK_DIALOG(dialog));
	if(resp == GTK_RESPONSE_OK) 
        {
	    fname = g_strdup(gtk_file_selection_get_filename(GTK_FILE_SELECTION(dialog)));
	    gtk_widget_destroy(dialog);
	} 
	else 
        {
	    gtk_widget_destroy(dialog);
	} 
    f1 = fopen(fname, "r");
    f2 = fopen("e.txt", "w"); 

    c = fgetc(f1);
printf("%s", key);
    while (c != EOF) 
    { 
          if(*key == '\0')
              key=keys;
          a = *key;
          c = c + a;
        fputc(c, f2);
          key++;
        c = fgetc(f1); 
    }
   fclose(f1);
   fclose(f2);  
}
void encryption(void){
GtkWidget *win;
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *key_button;
win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
gtk_window_set_title(GTK_WINDOW(win), "KEY");

  vbox = gtk_vbox_new (FALSE, 2);
  gtk_container_add (GTK_CONTAINER (win), vbox);

  hbox = gtk_hbox_new (FALSE, 2);

  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  
  key_entry = gtk_entry_new();
  
  gtk_box_pack_start (GTK_BOX (hbox),key_entry, TRUE, TRUE, 0);

  key_button = gtk_button_new_with_label ("keyno");
  gtk_box_pack_start (GTK_BOX (hbox), key_button, FALSE, FALSE, 0);
g_signal_connect(G_OBJECT (key_button), "clicked",G_CALLBACK (ec_clicked), NULL);
  
gtk_widget_show_all(win);

}
void dc_clicked ()
{
char *p;
  p = (gchar *)gtk_entry_get_text((GtkEntry *)key_entry);
   key = (char *)p;
   keys = (char *)p;
printf("%s", key);
printf("%s", keys);
char *fname;
int a;
 FILE *f1,*f2;
    char c;
printf("%s", key);
GtkWidget *dialog = gtk_file_selection_new("Share File...");
	int resp = gtk_dialog_run(GTK_DIALOG(dialog));
	if(resp == GTK_RESPONSE_OK) 
        {
	    fname = g_strdup(gtk_file_selection_get_filename(GTK_FILE_SELECTION(dialog)));
	    gtk_widget_destroy(dialog);
	} 
	else 
        {
	    gtk_widget_destroy(dialog);
	} 
    f1 = fopen(fname, "r");
    f2 = fopen("d.txt", "w"); 

    c = fgetc(f1);
printf("%s", key);
    while (c != EOF) 
    { 
          if(*key == '\0')
              key=keys;
          a = *key;
          c = c - a;
        fputc(c, f2);
          key++;
        c = fgetc(f1); 
    }
   fclose(f1);
   fclose(f2);  
}
void decryption(void){
GtkWidget *win;
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *key_button;
win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
gtk_window_set_title(GTK_WINDOW(win), "KEY");

  vbox = gtk_vbox_new (FALSE, 2);
  gtk_container_add (GTK_CONTAINER (win), vbox);

  hbox = gtk_hbox_new (FALSE, 2);

  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  
  key_entry = gtk_entry_new();
  
  gtk_box_pack_start (GTK_BOX (hbox),key_entry, TRUE, TRUE, 0);

  key_button = gtk_button_new_with_label ("keyno");
  gtk_box_pack_start (GTK_BOX (hbox), key_button, FALSE, FALSE, 0);
g_signal_connect(G_OBJECT (key_button), "clicked",G_CALLBACK (dc_clicked), NULL);
  
gtk_widget_show_all(win);

}

void load_file(char *fname)
{
    FILE *f; 

    GtkTextIter p;

    char fbuf[CHAR_BUF];
    size_t l;
    
    if(!fname) 
    {   
	GtkWidget *dialog = gtk_file_selection_new("Open File...");
	int resp = gtk_dialog_run(GTK_DIALOG(dialog));
	if(resp == GTK_RESPONSE_OK) 
        {   

	    fname = g_strdup(gtk_file_selection_get_filename(GTK_FILE_SELECTION(dialog)));
	    gtk_widget_destroy(dialog);
	} 
	else 
        {
	    gtk_widget_destroy(dialog);
	    return;
	}
    }

    if(!(f = fopen(fname, "r"))) 
    {
	g_printerr("%s: %s\n", fname, g_strerror(errno));
	return;
    }


    if(fname != filename) 
    {
	gchar *wt = g_strdup_printf("Text Editor (%s)", fname);
	g_free(filename);
	filename = fname;
	gtk_window_set_title(GTK_WINDOW(window), wt);
	g_free(wt);
    }

    gtk_text_buffer_get_end_iter(buf, &p);

    while((l = fread(fbuf, 1, sizeof(fbuf), f)) > 0) 
    {
    
	GError *err = NULL;
	gsize br, bw;
	gchar *text;
	if(!(text = g_locale_to_utf8(fbuf, l, &br, &bw, &err))) 
	{
	    g_printerr("File is not in UTF-8 format : %s\n", err->message);
	    g_clear_error(&err);
            filename = NULL;
	    gtk_window_set_title(GTK_WINDOW(window), "Text Editor (Insert file name)");
	    fclose(f);

	    return;
	}
	gtk_text_buffer_insert(buf, &p, text, bw);
	g_free(text);
    }

    
    gtk_text_buffer_set_modified(buf, FALSE);
    
    gtk_text_buffer_get_start_iter(buf, &p);
    gtk_text_buffer_place_cursor(buf, &p);
    if(ferror(f)) 
    {
	g_printerr("%s: %s\n", fname, g_strerror(errno));
	fclose(f);
	return;
    }
    if(fclose(f) == EOF)
	g_printerr("%s: %s\n", fname, g_strerror(errno));
} 

gboolean save_file(char *fname)
{
    FILE *f;
    int ok = TRUE;
    
    if(!fname) 
    {
	GtkWidget *dialog = gtk_file_selection_new("Save File As...");
	int resp = gtk_dialog_run(GTK_DIALOG(dialog));
	if(resp == GTK_RESPONSE_OK) {
	    fname = g_strdup(
		gtk_file_selection_get_filename(GTK_FILE_SELECTION(dialog)));
	    gtk_widget_destroy(dialog);
	} else {
	    gtk_widget_destroy(dialog);
	    return FALSE;
	}
    }

    if(!(f = fopen(fname, "w")))
    {
	g_printerr("%s: %s\n", fname, g_strerror(errno));
	ok = FALSE;
    } else {
	GtkTextIter start, end, p;

	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buf), &start, &end);
	p = start;
	while(!gtk_text_iter_equal(&start, &end)) {
	    gchar *buf, *fbuf;
	    gsize br, bw;
	    GError *err = NULL;
	    gtk_text_iter_forward_chars(&p, CHAR_BUF);
	    buf = gtk_text_iter_get_slice(&start, &p);
	    fbuf = g_locale_from_utf8(buf, -1, &br, &bw, &err);
	    g_free(buf);

	    if(!fbuf) 
            {
		g_printerr("Failed UTF-8 to locale conversion: %s\n",
			   err->message);
		g_clear_error(&err);
		ok = FALSE;
		break;
	    }
	    fwrite(fbuf, bw, 1, f);
	    g_free(fbuf);
	    if(ferror(f)) {
		g_printerr("%s: %s\n", fname, g_strerror(errno));
		ok = FALSE;
		break;
	    }
	    start = p;
	}
	if(fclose(f) == EOF) 
        {
	    g_printerr("%s: %s\n", fname, g_strerror(errno));
	    ok = FALSE;
	}
    }
    
    if(ok) 
    {
	gtk_text_buffer_set_modified(buf, FALSE);
	if(fname != filename) {
	    gchar *wt = g_strdup_printf("TextView (%s)", fname);
	    g_free(filename);
	    filename = fname;
	    gtk_window_set_title(GTK_WINDOW(window), wt);
	    g_free(wt);
	}
    }
    return ok;
}


gboolean save_if_modified(void)
{
    int resp;
    GtkWidget *dialog;

    if(!gtk_text_buffer_get_modified(GTK_TEXT_BUFFER(buf)))
	return TRUE;

    dialog = gtk_message_dialog_new(GTK_WINDOW(window), 0, GTK_MESSAGE_QUESTION,
				    GTK_BUTTONS_NONE,
				    "Buffer Modified. Do you want to save it now?");
    gtk_dialog_add_buttons(GTK_DIALOG(dialog), GTK_STOCK_YES, GTK_RESPONSE_YES,
			   GTK_STOCK_NO, GTK_RESPONSE_NO,
			   GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
    resp = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    switch(resp) {
    case GTK_RESPONSE_YES:
	return save_file(filename);
    case GTK_RESPONSE_NO:
	return TRUE;
    case GTK_RESPONSE_CANCEL:
    case GTK_RESPONSE_DELETE_EVENT:
	return FALSE;
    default:
	g_printerr("Error response %d from dialog\n", resp);
	return FALSE;
    }
}

void spell_clicked(void){
GtkTextIter start, end;
gchar *fbuf;
FILE *f1;
char buffer[512];
int lines=0;
char c;
int ch=0;
char *word; 
  hashtable_t *hashtable = ht_create(393241);  

f1 = fopen("dict.txt", "r");

while((c=getc(f1))!=EOF)
	{	
		if(c == '\n')
		lines++;	
	} 
rewind(f1);
while(ch != lines)
                          {
		fscanf(f1 , "%[^\n]%*c" , buffer);
                ht_set(hashtable, buffer, buffer);
                bzero(buffer,512);
            ch++;
      }
gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buf), &start, &end);
fbuf = gtk_text_buffer_get_text(buf,&start, &end, FALSE);

    word = strtok(fbuf, SEPCHARS);

    while (word != NULL) {
       if (ht_get(hashtable, word) == NULL) {
        printf("%s not match\n",word);
      }
      word = strtok(NULL, SEPCHARS);
    }

 
}

void menu_show(gpointer data, guint action, GtkWidget *widget){
         GtkTextIter p;
        switch(action)
            {
           case MENU_NEW:
           if(save_if_modified())
{
	  buf = gtk_text_buffer_new(gtk_text_buffer_get_tag_table(buf));
	  gtk_text_view_set_buffer(GTK_TEXT_VIEW(view), buf);
	  g_object_unref(G_OBJECT(buf));
	}
	break;
 case MENU_OPEN:
	if(save_if_modified()) 
        {
	  buf = gtk_text_buffer_new(gtk_text_buffer_get_tag_table(buf));
	  gtk_text_view_set_buffer(GTK_TEXT_VIEW(view), buf);
	  g_object_unref(G_OBJECT(buf));
	  load_file(NULL);
	}
	break;
      case MENU_SAVE:
	save_file(filename);
	break;
      case MENU_SAVE_AS:
	save_file(NULL);
	break;
      case MENU_QUIT:
	if(save_if_modified())
	  gtk_widget_destroy(window);
	break;
      case MENU_CUT:
	gtk_text_buffer_cut_clipboard(buf,gtk_clipboard_get(sel_atom), TRUE);
	break;
      case MENU_COPY:
	gtk_text_buffer_copy_clipboard(buf,gtk_clipboard_get(sel_atom));
	break;
      case MENU_PASTE:
	gtk_text_buffer_paste_clipboard(buf,gtk_clipboard_get(sel_atom), NULL, TRUE);
	break;
      case MENU_SELECT_ALL:
	gtk_text_buffer_get_start_iter(buf, &p);  
	gtk_text_buffer_place_cursor(buf, &p);
	gtk_text_buffer_get_end_iter(buf, &p);
	gtk_text_buffer_move_mark_by_name(buf, "selection_bound", &p);
	break;
      case MENU_HELP:
	show_help();
	break;
      case MENU_ABOUT:
        show_about();
        break;
       case MENU_SERVER_PORTNO:
            pn();
          break;
       case MENU_SERVER:
           servert();
          break;
       case MENU_SEND_SERVER:
            sends();
          break;
       case MENU_RECIEVE_SERVER:
           recieves();
          break;
       case MENU_CLIENT_PORTNO:
          pn();
          break;
       case MENU_IP:
          ip();
          break;
       case MENU_CLIENT:
          client();
          break;
       case MENU_SEND_CLIENT:
          sendc();
          break;
        case MENU_RECIEVE_CLIENT:
          recievec();
          break;
        case MENU_ENCRYPTION:
          encryption();
          break;
        case MENU_DECRYPTION:
          decryption();
          break;
        case MENU_SPELL:
          spell_clicked();
          break;
        default:
	break;
      }
} 

GtkItemFactoryEntry menu_array[] = 
  {
    { (char *)"/_File", NULL, NULL, 0, (char *)"<Branch>", NULL },
    { (char *)"/File/_New", (char *)"<control>N", menu_show, MENU_NEW, "<Item>"},
    { (char *)"/File/_Open...", (char *)"<control>O", menu_show, MENU_OPEN, "<Item>" },
    { (char *)"/File/_Save", (char *)"<control>S", menu_show, MENU_SAVE, "<Item>"},
    { (char *)"/File/Save _As...", NULL, menu_show, MENU_SAVE_AS, "<Item>"},
    { (char *)"/File/_Quit", (char *)"<control>Q", menu_show, MENU_QUIT, "<Item>"},
    { (char *)"/_Edit", NULL, NULL, 0, (char *)"<Branch>", NULL },
    { (char *)"/Edit/_Cut", (char *)"<control>X", menu_show, MENU_CUT, "<Item>"},
    { (char *)"/Edit/_Copy", (char *)"<control>C", menu_show, MENU_COPY, "<Item>" },
    { (char *)"/Edit/_Paste", (char *)"<control>V", menu_show, MENU_PASTE, "<Item>"},
    { (char *)"/Edit/Select All", NULL, menu_show, MENU_SELECT_ALL, "<Item>"},
    { (char *)"/_Help", NULL, NULL, 0, (char *)"<Branch>", NULL },
    { (char *)"/Help/_Contents", (char *)"<control>H", menu_show, MENU_HELP,"<Item>"},
    { (char *)"/Help/_About",  NULL , menu_show, MENU_ABOUT, "<Item>" },
    { (char *)"/_Server", NULL, NULL, 0, (char *)"<Branch>", NULL },
    { (char *)"/Server/_PortNo", NULL, menu_show, MENU_SERVER_PORTNO, "<Item>" },
    { (char *)"/Server/_Connect", NULL, menu_show, MENU_SERVER, "<Item>" },
 { (char *)"/Server/_Send", NULL, menu_show, MENU_SEND_SERVER, "<Item>" },
{ (char *)"/Server/_Recieve", NULL, menu_show, MENU_RECIEVE_SERVER, "<Item>" },
{ (char *)"/_Client", NULL, NULL, 0, (char *)"<Branch>", NULL },
{ (char *)"/Client/_PortNo", NULL, menu_show, MENU_CLIENT_PORTNO, "<Item>" },
{ (char *)"/Client/_IP", NULL, menu_show, MENU_IP, "<Item>" },
{ (char *)"/Client/_Connect", NULL, menu_show, MENU_CLIENT, "<Item>" },
{ (char *)"/Client/_Send", NULL, menu_show, MENU_SEND_CLIENT, "<Item>" },
{ (char *)"/Client/_Recieve", NULL, menu_show, MENU_RECIEVE_CLIENT, "<Item>" },
{ (char *)"/_Security", NULL, NULL, 0, (char *)"<Branch>", NULL },
{ (char *)"/Security/_Encryption", NULL, menu_show, MENU_ENCRYPTION, "<Item>" },
{ (char *)"/Security/_Decryption", NULL, menu_show, MENU_DECRYPTION, "<Item>" },
{ (char *)"/_Spell", NULL, NULL, 0, (char *)"<Branch>", NULL },
{ (char *)"/Spell/_Spellcheck", NULL, menu_show, MENU_SPELL, "<Item>" },
 };
int main(int argc, char *argv[])
{
GtkWidget *box, *scroll, *widget;
GtkAccelGroup *accel;
gtk_init(&argc, &argv);
window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
gtk_window_set_title(GTK_WINDOW(window), "TEXTEDITOR");
gtk_window_set_default_size(GTK_WINDOW(window), 600, 500);
g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
box = gtk_vbox_new(FALSE,0);
gtk_container_add(GTK_CONTAINER(window), box);
accel = gtk_accel_group_new();
gtk_window_add_accel_group(GTK_WINDOW(window), accel);
/*container_type the kind of menu to create; can be GTK_TYPE_MENU_BAR, GTK_TYPE_MENU or GTK_TYPE_OPTION_MENU
path  the factory path of the new item factory, a string of the form "<name>"
accel_groupa GtkAccelGroup to which the accelerators for the menu items will be added, or NULL to create a new one.[allow-none]
returns a new GtkItemFactor */
main_menu = gtk_item_factory_new(GTK_TYPE_MENU_BAR, "<TextView>", accel);
gtk_item_factory_create_items(main_menu,sizeof(menu_array) / sizeof(*menu_array),menu_array, NULL);
widget = gtk_item_factory_get_widget(main_menu, "<TextView>");
gtk_box_pack_start(GTK_BOX(box), widget, FALSE, FALSE, 0);
scroll = gtk_scrolled_window_new(NULL, NULL);
gtk_box_pack_start(GTK_BOX(box), scroll, TRUE, TRUE, 0);
view = gtk_text_view_new();
buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
gtk_container_add(GTK_CONTAINER(scroll), view);
gtk_widget_show_all(window);
gtk_main();
return 0;
}

