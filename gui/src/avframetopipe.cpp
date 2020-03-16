/*
 * This file is part of Chiaki.
 *
 * Chiaki is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Chiaki is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Chiaki.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <avframetopipe.h>
#include <avopenglwidget.h>
#include <videodecoder.h>

#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include <stdio.h> //fopen, write

AVFrameToPipe::AVFrameToPipe(VideoDecoder *decoder)
	: QObject(nullptr),
	decoder(decoder)
{
	connect(decoder, SIGNAL(FramesAvailable()), this, SLOT(FrameToPipe()));
}

void AVFrameToPipe::FrameToPipe()
{
	AVFrame *frame = decoder->PullFrame();
	if(!frame)
		return;

	//check that the frame is of expected type (YUV 4:2:0 planar)
	if(frame->format != AV_PIX_FMT_YUV420P)
	{
		printf("AVFrameToPipe got AVFrame with invalid format\n");
		return;
	}

	//now open and write Y,U,V data to 3 different fifos

	int width = frame->width;
	int height = frame->height;
	
	int halfwidth = width >> 1;
	int halfheight = height >> 1;
	
	//TODO don't hardcode fifo names
	constexpr char const* fifo_names[] = {"/tmp/fifos/y_fifo", "/tmp/fifos/u_fifo", "/tmp/fifos/v_fifo"};
	
	//write frame pixel data to fifos
	for (int i = 0; i < 3; ++i)
	{
		int h = i > 0 ? halfheight : height;
		int w = i > 0 ? halfwidth : width;
		
		FILE* f = fopen(fifo_names[i], "wb");
		if (NULL == f)
		{
			printf("AVFrameToPipe could not open named pipes\n");
			return;
		}
		
		if(frame->linesize[i] == w)
			fwrite(frame->data[i], w * h, 1, f);
		else
		{
			for(int l=0; l<h; l++)
				fwrite(frame->data[i] + frame->linesize[i] * l, w, 1, f);
		}
		
		fclose(f);
	}
	av_frame_free(&frame);
}
