#include <gtk/gtk.h>
#include "gtkApp.h"

static void 
on_entry_activate(GtkEntry *entry, gpointer user_data) {
	const gchar *command;
	GtkTextView *text_view;
	GtkTextBuffer *buffer;
	GtkTextIter end;
	
	command = gtk_editable_get_text(GTK_EDITABLE(entry));
	gtk_editable_set_text(GTK_EDITABLE(entry), ""); // Svuota la entry
	
	text_view = GTK_TEXT_VIEW(user_data);
	buffer = gtk_text_view_get_buffer(text_view);
	
	gtk_text_buffer_get_end_iter(buffer, &end);
	
	gtk_text_buffer_insert(buffer, &end, "$ ", -1);
	
	GtkTextIter start;
	gtk_text_buffer_get_end_iter(buffer, &start);
	
	gtk_text_buffer_insert(buffer, &end, command, -1);
	
	GtkTextIter command_end;
	gtk_text_buffer_get_end_iter(buffer, &command_end);
	gtk_text_buffer_apply_tag_by_name(buffer, "green_text", &start, &command_end);
	
	gtk_text_buffer_insert(buffer, &end, "\n", -1);	
	
}
static void
activate (GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *scrolled;
    GtkWidget *text_view;
    GtkWidget *entry;

    // Crea la finestra
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Custom Shell");
    gtk_window_set_default_size (GTK_WINDOW (window), 400, 300);

    // Crea contenitore verticale
    vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_window_set_child (GTK_WINDOW(window), vbox);

   	// Crea Scroll + TextView
   	scrolled = gtk_scrolled_window_new ();
    gtk_widget_set_vexpand(scrolled, TRUE);
    gtk_box_append (GTK_BOX (vbox), scrolled);
    
    text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), text_view);
   
   	// Crea e aggiunge un tag "green_text" al buffer
   	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
   	GtkTextTagTable *tag_table = gtk_text_buffer_get_tag_table(buffer);
   	GtkTextTag *green_tag = gtk_text_tag_new("green_text");
   	g_object_set(green_tag, "foreground", "green", NULL);
   	gtk_text_tag_table_add(tag_table, green_tag);
   	
    // Crea GtkEntry
    entry = gtk_entry_new ();
    gtk_box_append (GTK_BOX (vbox), entry);
    
    g_signal_connect(entry, "activate", G_CALLBACK(on_entry_activate), text_view);

    gtk_window_present (GTK_WINDOW (window));
}

// Inizializzazione GUI, creazione "app", chiamata ad "activate()"...
int create_app(int argc, char **argv) {
	GtkApplication *app;
	int status;
	
	app = gtk_application_new(
		"it.pasciu.aliss.customshell", 
		G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	
	return status;
}
