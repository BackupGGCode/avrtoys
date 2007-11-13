FILTER=dot
PSVIEW=kpdf
VIEW=gwenview

TARGET=steps

all: svg ps dia

view: viewsvg


dosvg: svg viewsvg

svg:
	$(FILTER) $(TARGET).dot -Tsvg -o $(TARGET).svg

viewsvg: svg
	$(VIEW) $(TARGET).svg

# PS
dops: ps viewps

ps:
	$(FILTER) $(TARGET).dot -Tps -o $(TARGET).ps

viewps: ps
	$(PSVIEW) $(TARGET).ps

dia:
	$(FILTER) $(TARGET).dot -Tdia -o $(TARGET).dia

clean:
	rm -f $(TARGET).ps $(TARGET).svg $(TARGET).dia

