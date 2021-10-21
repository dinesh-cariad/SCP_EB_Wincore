#!/bin/sh

cd ..
TRESOS_BASE="$(pwd)"

#JAVA setting
JAVACMD=java
JAVA_HOME=$TRESOS_BASE/jre_x86_64/linux
JAVA=$JAVA_HOME/bin/$JAVACMD

#PATH setting
SAVED_PATH=$PATH

#Command Line setting
CLASSPATH=$TRESOS_BASE/plugins/org.eclipse.equinox.launcher_1.3.201.v20161025-1711.jar
APPLICATION=dreisoft.tresos.generator.ng.api.plugin.jetcompiler
VMARGS="-server -Dbase.path="$TRESOS_BASE" -Djava.library.path="$TRESOS_BASE"/"lib
ARGS="-nl en -install "$TRESOS_BASE"/"
LAUNCHER=org.eclipse.equinox.launcher.Main
CMD=""$JAVA" $VMARGS -classpath $CLASSPATH $LAUNCHER -application $APPLICATION $ARGS $@ "
$CMD

