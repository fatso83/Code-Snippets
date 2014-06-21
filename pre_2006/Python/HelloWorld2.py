#!/usr/bin/env python
import gtk


class HelloWorld2:

    def callback(self, widget, data):
        print "%s was pressed" %data

    def delete_event(self, widget, event, data=None):
        gtk.main_quit()

        #If returned True the main window should not be destroyed
        # But what happens when you already called gtk.main_quit() ?
        return False

    def destroy(self, widget, data=None):
        gtk.main_quit()

    def __init__(self):
        self.window = gtk.Window()
        self.window.set_title("HelloWorld 2 test program")

        # immediately exit GTK
        self.window.connect("delete_event", self.delete_event)
        self.window.connect("destroy", self.destroy)
        
        # borderwidth
        self.window.set_border_width=0

        # a box to vertically pack widgets into
        self.box1 = gtk.VBox(False, 0)
        self.window.add(self.box1)

        # horizontally pack widgets
        
        #HBox( take_up_same_amount_of_space, spacing)
        self.buttonbox = gtk.HBox(False, 20)
        #pack_start(button, center, fillbox, padding)
        self.box1.pack_start(self.buttonbox, True, True, 0)
        self.buttonbox.show()

        self.button1 = gtk.Button("Button 1")
        self.button1.connect("clicked", self.callback, "Button 1")
        self.buttonbox.pack_start( self.button1, False, False, 0 )
        
        self.button1.show()

        self.button2 = gtk.Button("Button 2")
        self.button2.connect("clicked", self.callback, "Button 2")
        self.buttonbox.pack_start(self.button2, True, False, 0)
        self.button2.show()

        self.button_quit = gtk.Button("Quit", gtk.STOCK_QUIT)
        self.button_quit.connect("clicked", self.callback, "Quit button")
        # the button will send a destroy signal. handle the signal to quit
        self.button_quit.connect_object("clicked", gtk.Widget.destroy, self.window)
        self.box1.pack_start(self.button_quit, True, True, 0)
        self.button_quit.show()
        # The order in which we show the buttons is not important...

        self.box1.show()
        self.window.show()

def main():
    gtk.main()

baseclass=HelloWorld2()
main()
