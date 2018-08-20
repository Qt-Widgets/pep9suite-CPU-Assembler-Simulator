// File: asmprogrammanager.h
/*
    Pep9 is a virtual machine for writing machine language and assembly
    language programs.

    Copyright (C) 2018 Matthew McRaven, Pepperdine University

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef ASMPROGRAMMANAGER_H
#define ASMPROGRAMMANAGER_H
#include <QObject>
#include <QSharedPointer>
#include <QSet>
class AsmProgram;
class SymbolTable;
/*
 * A class to manage the lifetime of user programs & the operating system.
 * Also helps communicate changes in breakpoints between assembler source &
 * listing panes, and the CPU Control Section.
 * It can also translate from a memory address to the assembly code program which contains that address.
 */
class AsmProgramManager: public QObject
{
    Q_OBJECT
public:

    static AsmProgramManager* getInstance();
    QSharedPointer<AsmProgram> getOperatingSystem();
    QSharedPointer<AsmProgram> getUserProgram();
    QSharedPointer<const AsmProgram> getOperatingSystem() const;
    QSharedPointer<const AsmProgram> getUserProgram() const;
    void setOperatingSystem(QSharedPointer<AsmProgram> prog);
    void setUserProgram(QSharedPointer<AsmProgram> prog);
    const AsmProgram* getProgramAtPC() const;
    AsmProgram* getProgramAtPC();
    const AsmProgram* getProgramAt(quint16 address) const;
    AsmProgram* getProgramAt(quint16 address);
    QSet<quint16> getBreakpoints() const;
public slots:
    void onBreakpointAdded(quint16 address);
    void onBreakpointRemoved(quint16 address);
    void onRemoveAllBreakpoints();

signals:
    void breakpointAdded(quint16 address);
    void breakpointRemoved(quint16 address);
    void removeAllBreakpoints();
    void setBreakpoints(QSet<quint16> addresses);

private:
    AsmProgramManager(QObject* parent = nullptr);
    static AsmProgramManager* instance;
    QSharedPointer<AsmProgram> operatingSystem;
    QSharedPointer<AsmProgram> userProgram;
};

#endif // ASMPROGRAMMANAGER_H
