#!/usr/bin/env python
# -*- encoding: utf-8 -*-
import pygtk
import gtk

class HelloWorld:

    def delete_event(self, widget, event, data=None):
        #change return status to True and the main window
        #will not be destroyed
        ret_status = True
        
        print "delete_event occured"
        print "Return status set to ", ret_status

        return ret_status

    def destroy(self, widget, data=None):
        gtk.main_quit()

    def hello( self, widget, alt_tekst = None ):
        if alt_tekst == None:
            print "Hallo verden"
        else:
            print alt_tekst            

        print "self=", self, "\nwidget=",widget

    def inFocus(widget, event, cb_data):
        print "event=",event
        print "widget",widget
        print "We're in focus!"

        return True

    def outFocus(widget, event, cb_data):
        print "event=",event
        print "widget",widget
        print "We are out of focus... :("

        return True

    def main(self):
        gtk.main()

    def __init__(self):
        self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
        # close, Alt-F4, eller "X"
        self.window.connect("delete_event", self.delete_event)

        # connect "destroy" event to signal handler.
        # happens when we call gtk.widget_destroy() on the window,
        # or if we return FALSE in the "delete_event" callback
        self.window.connect("destroy", self.destroy)

        # set borderwidth
        self.window.set_border_width(20)

        self.button = gtk.Button("Trykk på meg!")
        # callback when "clicked"
        self.button.connect("clicked", self.hello, "En litt alternativ tekst")

        # destroys the window on "clicked"
        self.button.connect_object("clicked", gtk.Widget.destroy, self.window)
        #self.button.connect_object("clicked", gtk.Widget.destroy, self.button)

        # add button to window
        self.window.add(self.button)

        self.button.show()
        self.window.show()

        # handling of X-events, focus_in_event, focus_out_event
        self.window.connect("focus_in_event", self.inFocus)
        self.window.connect("focus_out_event", self.outFocus)
        
        
#
# HelloWorld class def ends
#


print __name__
if __name__ == "__main__":
    base = HelloWorld()
    base.main()
