# -*- encoding: utf-8 -*-

import pygtk
pygtk.require('2.0')
import gtk

def callback_button_pressed(button):
    """Outputs a message when the button is pressed

    Takes a gtk.Button objekt as argument"""

    button.pressedCount += 1
    print "Knappen ble trykket ned for", button.pressedCount, ".gang."

class Button (gtk.Button):
    pressedCount = 0


window = gtk.Window()
window.set_title("Mitt første vindu")
button = Button("Press me")
button.connect("clicked", callback_button_pressed)
window.add(button)
window.show_all()
gtk.main()

    
