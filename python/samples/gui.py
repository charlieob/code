import Tkinter
 
# Define input retrieve function for application input
def retrieve_text():
   print app_entry.get()
 
if __name__ == "__main__":
 
   # Create Window Object or Window Form
   app_win = Tkinter.Tk()
 
   # Create label object
   app_label = Tkinter.Label(app_win,text="Hello World")
   app_label.pack()
 
   # Create User Input Object
   app_entry = Tkinter.Entry(app_win)
   app_entry.pack()
 
   # Create Button Object
   app_button = Tkinter.Button(app_win,text="Print Value",command=retrieve_text)
   app_button.pack()
 
   # Initialize Graphical Loop
   app_win.mainloop()
