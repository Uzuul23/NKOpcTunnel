/*	This file is part of NKOpcTunnel.
*
*	Copyright (c) Henryk Anschuetz 
*	Berlin, Germany
*
*	mailto:uzuul23@online.de
*
*	NKOpcTunnel is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   NKOpcTunnel is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with NKOpcTunnel.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#pragma once

// Die folgenden Makros definieren die mindestens erforderliche Plattform. Die mindestens erforderliche Plattform
// ist die früheste Windows-, Internet Explorer-Version usw., die über die erforderlichen Features zur Ausführung 
// Ihrer Anwendung verfügt. Die Makros aktivieren alle Funktionen, die auf den Plattformversionen bis 
// einschließlich der angegebenen Version verfügbar sind.

// Ändern Sie folgende Definitionen für Plattformen, die älter als die unten angegebenen sind.
// Unter MSDN finden Sie die neuesten Informationen über die entsprechenden Werte für die unterschiedlichen Plattformen.
#ifndef WINVER                          // Gibt an, dass Windows Vista die mindestens erforderliche Plattform ist.
#define WINVER 0x0600           // Ändern Sie den entsprechenden Wert, um auf andere Versionen von Windows abzuzielen.
#endif

#ifndef _WIN32_WINNT            // Gibt an, dass Windows Vista die mindestens erforderliche Plattform ist.
#define _WIN32_WINNT 0x0600     // Ändern Sie den entsprechenden Wert, um auf andere Versionen von Windows abzuzielen.
#endif

#ifndef _WIN32_WINDOWS          // Gibt an, dass Windows 98 die mindestens erforderliche Plattform ist.
#define _WIN32_WINDOWS 0x0410 // Ändern Sie den entsprechenden Wert, um auf mindestens Windows Me abzuzielen.
#endif

#ifndef _WIN32_IE                       // Gibt an, dass Internet Explorer 7.0 die mindestens erforderliche Plattform ist.
#define _WIN32_IE 0x0700        // Ändern Sie den entsprechenden Wert, um auf andere Versionen von IE abzuzielen.
#endif
