import pywinauto, time
from pywinauto import Desktop
from pywinauto.application import Application
from pywinauto.keyboard import send_keys

app = Application().start(r"C:\SierraChart\SierraChart_64.exe", timeout=10)
app = Application().connect(path=r"C:\SierraChart\SierraChart_64.exe")
dlg = app.window(title_re=r"Sierra Chart")
dlg.TabControl.click()

