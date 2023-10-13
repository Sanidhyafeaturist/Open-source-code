void CodeEditor::compileAndRunCode() {
    QString code = textEdit->toPlainText();

    // Compile and run the code using an external compiler (e.g., g++)
    QProcess compilerProcess;
    QString compilerCmd = "g++"; // Modify this with your compiler command
    QStringList args;
    args << "-o" << "output.exe" << "-xc++" << "-"; // Specify source code from stdin

    compilerProcess.setProgram(compilerCmd);
    compilerProcess.setArguments(args);
    compilerProcess.setStandardInputFile("-");

    // Start the compiler process
    compilerProcess.start();
    compilerProcess.write(code.toUtf8()); // Pass the code as input
    compilerProcess.closeWriteChannel();
    compilerProcess.waitForFinished();

    // Check the compiler exit code for success or failure
    int exitCode = compilerProcess.exitCode();
    if (exitCode == 0) {
        // Compilation succeeded, now run the program
        QProcess runProcess;
        runProcess.setProgram("output.exe");

        // Start the program execution
        runProcess.start();
        runProcess.waitForFinished();

        // Display the program's output
        QString programOutput = runProcess.readAllStandardOutput();
        textEdit->appendPlainText("\nProgram Output:\n" + programOutput);
    }
    else {
        // Compilation failed
        QString errorOutput = compilerProcess.readAllStandardError();
        QMessageBox::critical(this, "Error", "Compilation failed:\n\n" + errorOutput);
    }
}
